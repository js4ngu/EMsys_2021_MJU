#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/miscdevice.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <asm/uaccess.h>

#include "fnddrv.h"

/*
              0
             ---
		  5 |   | 1
			| 6 |
			 ---
		  4 |   | 2
            | 3	|		
			 ---   * 7
              
*/

#define  LOW 		0
#define  HIGH		1

#define  SELECT_ON   LOW
#define  SELECT_OFF  HIGH
//#define  SELECT_ON   HIGH
//#define  SELECT_OFF  LOW
 
#define  FND_LED_ON  HIGH
#define  FND_LED_OFF LOW

#define DISPLAY_MAX_NUM		10
#define FND_LED_NUM_WITHOUT_DP	7

#define FND_LED_DP_INDEX	7

#define FND_LED_GPIO_NUM	8		

#define DRIVER_NAME              "perifnd"
#define PINCTRL_DEFAULT          "default"

struct perifnd_data {
	struct pinctrl *pinctrl;	
	struct pinctrl_state *pins_default;
	struct mutex				lock;
	struct hrtimer display_timer;
	int    init_delay;
	int	   select_interval;

	int    gpio_fndled[FND_LED_GPIO_NUM];
	int    gpio_fndSelect[MAX_FND_NUM];
	
	stFndWriteForm 		stCurFndData;
	
	
	int  currentSelectIndex;
};

static struct platform_device_id perifnd_devtype[] = {
	{
		.name = DRIVER_NAME,
		.driver_data = 0,
	}, {
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(platform, perifnd_devtype);

static const struct of_device_id perifnd_dt_ids[] = {
	{ .compatible = "cndi,perifnd", .data = &perifnd_devtype[0], },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, perifnd_dt_ids);

static ssize_t perifnd_write(struct file *file, const char __user *data,
                             size_t len, loff_t *ppos);
							 
static const struct file_operations perifnd_fops = {
	.write =                perifnd_write,
};

static struct miscdevice perifnd_miscdev = {
	.minor =        MISC_DYNAMIC_MINOR,
	.name =         "perifnd",
	.fops =         &perifnd_fops,
};

int  numericFndData[DISPLAY_MAX_NUM][FND_LED_NUM_WITHOUT_DP] =
{
	// '0'
	{ 1, 1, 1, 1, 1, 1, 0 },
	// '1'
	{ 0, 1, 1, 0, 0, 0, 0 },
	// '2'
	{ 1, 1, 0, 1, 1, 0, 1 },
	// '3'
	{ 1, 1, 1, 1, 0, 0, 1 },
	// '4'
	{ 0, 1, 1,0, 0, 1, 1 },
	// '5'
	{ 1, 0, 1, 1, 0, 1, 1 },
	// '6'
	{ 1, 0, 1, 1, 1, 1, 1 },
	// '7'
	{ 1, 1, 1, 0, 0, 0, 0 },
	// '8'
	{ 1, 1, 1, 1, 1, 1, 1 },
	// '9'
	{ 1, 1, 1, 1, 0, 1, 1 }
};

static int perifnd_next_fnd_display(struct perifnd_data *pData)
{
	int ret; 
	int i; 
	int characterIndex;
	ret = mutex_lock_interruptible(&pData->lock);
	if (ret < 0)
		return ret;
	
	gpio_set_value(pData->gpio_fndSelect[pData->currentSelectIndex], SELECT_OFF);
	
	pData->currentSelectIndex++;
	if (pData->currentSelectIndex >=MAX_FND_NUM  )
	{
		pData->currentSelectIndex = 0;
	}
	if ( pData->stCurFndData.DataValid[pData->currentSelectIndex])
	{
		characterIndex = pData->stCurFndData.DataNumeric[pData->currentSelectIndex];
		for( i = 0; i < FND_LED_NUM_WITHOUT_DP ; i++ )
		{
			if( numericFndData[characterIndex][i] )
			{
				
				gpio_set_value(pData->gpio_fndled[i], FND_LED_ON);
			}
			else
			{
				gpio_set_value(pData->gpio_fndled[i], FND_LED_OFF);
			}
		}
		if (pData->stCurFndData.DataDot[pData->currentSelectIndex])
		{
			gpio_set_value(pData->gpio_fndled[FND_LED_DP_INDEX], FND_LED_ON);
		}
		else
		{
			gpio_set_value(pData->gpio_fndled[FND_LED_DP_INDEX], FND_LED_OFF);
		}
		
		gpio_set_value(pData->gpio_fndSelect[pData->currentSelectIndex], SELECT_ON);
	}
	
	mutex_unlock(&pData->lock);
	
	return 0; 
}
static struct perifnd_data *pPerifndData;
static enum hrtimer_restart perifnd_periodic_work(struct hrtimer *timer)
{
	ktime_t currtime , interval;

	if ( pPerifndData != NULL)
		perifnd_next_fnd_display(pPerifndData);
 	currtime  = ktime_get();
  	interval = ktime_set(0,4000000); 
	
  	hrtimer_forward(timer, currtime , interval);
//	printk("%s enter\n", __func__);
	return HRTIMER_RESTART;
}

static ssize_t perifnd_write(struct file *file, const char __user *data,
                             size_t len, loff_t *ppos)
{
	long res; 

	int i;

	stFndWriteForm stWriteData;
	struct miscdevice* md = &perifnd_miscdev;
	struct perifnd_data *pData ;
	
	pData = dev_get_drvdata(md->parent);
	

	if ( len <  sizeof (stFndWriteForm))
		return 0;

	res = copy_from_user(&stWriteData, data, sizeof (stFndWriteForm));
	
	if (res)
	{
		return -EFAULT;
	}
    // check numeric data 0 ~ 9
	for( i = 0; i < MAX_FND_NUM ; i++)
	{
		if ((stWriteData.DataNumeric[i] < 0) ||( stWriteData.DataNumeric[i] > 9 )) 
		{
			return 0; 
		}
	}
	memcpy(&pData->stCurFndData,&stWriteData, sizeof(stFndWriteForm));	

	return len;								 
}


static void perifnd_initVariable(struct perifnd_data *pData)
{
	int i;
	
	pData->init_delay =  200;
	pData->select_interval =  4;
	memset(&pData->stCurFndData, 0, sizeof(stFndWriteForm));
	pData->currentSelectIndex = 0;
	
	for(i = 0; i < 6 ; i++)
	{
		pData->stCurFndData.DataValid[i] = 1;
		pData->stCurFndData.DataNumeric[i] = i;
	}
}

static int setup_perifnd_gpio(struct perifnd_data *pData)  
{
	int ret;
	
//	int    gpio_fndled[FND_LED_GPIO_NUM];
//	int    gpio_fndSelect[MAX_FND_NUM];
	
	ret = gpio_request(pData->gpio_fndled[0], "perifnd fndled 0");
	if (ret) {
		printk(KERN_ERR "request perifnd 0 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndled[0] , FND_LED_OFF );
	}
	
	ret = gpio_request(pData->gpio_fndled[1], "perifnd fndled 1");
	if (ret) {
		printk(KERN_ERR "request perifnd 1 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndled[1] , FND_LED_OFF );
	}
	
	ret = gpio_request(pData->gpio_fndled[2], "perifnd fndled 2");
	if (ret) {
		printk(KERN_ERR "request perifnd 2 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndled[2] , FND_LED_OFF );
	}
	
	ret = gpio_request(pData->gpio_fndled[3], "perifnd fndled 3");
	if (ret) {
		printk(KERN_ERR "request perifnd 3 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndled[3] , FND_LED_OFF );
	}
	
	ret = gpio_request(pData->gpio_fndled[4], "perifnd fndled 4");
	if (ret) {
		printk(KERN_ERR "request perifnd 4 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndled[4] , FND_LED_OFF );
	}
	
	ret = gpio_request(pData->gpio_fndled[5], "perifnd fndled 5");
	if (ret) {
		printk(KERN_ERR "request perifnd 5 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndled[5] , FND_LED_OFF );
	}

	ret = gpio_request(pData->gpio_fndled[6], "perifnd fndled 6");
	if (ret) {
		printk(KERN_ERR "request perifnd 6 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndled[6] , FND_LED_OFF );
	}

	ret = gpio_request(pData->gpio_fndled[7], "perifnd fndled 7");
	if (ret) {
		printk(KERN_ERR "request perifnd 7 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndled[7] , FND_LED_OFF );
	}

	
	ret = gpio_request(pData->gpio_fndSelect[0], "perifnd select 0");
	if (ret) {
		printk(KERN_ERR "request perifnd s0 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndSelect[0] , SELECT_OFF );
	}
	
	ret = gpio_request(pData->gpio_fndSelect[1], "perifnd select 1");
	if (ret) {
		printk(KERN_ERR "request perifnd s1 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndSelect[1] , SELECT_OFF );
	}
	
	ret = gpio_request(pData->gpio_fndSelect[2], "perifnd select 2");
	if (ret) {
		printk(KERN_ERR "request perifnd s2 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndSelect[2] , SELECT_OFF );
	}
	
	ret = gpio_request(pData->gpio_fndSelect[3], "perifnd select 3");
	if (ret) {
		printk(KERN_ERR "request perifnd s3 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndSelect[3] , SELECT_OFF );
	}
	
	ret = gpio_request(pData->gpio_fndSelect[4], "perifnd select 4");
	if (ret) {
		printk(KERN_ERR "request perifnd s4 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndSelect[4] , SELECT_OFF );
	}
	
	ret = gpio_request(pData->gpio_fndSelect[5], "perifnd select 5");
	if (ret) {
		printk(KERN_ERR "request perifnd s5 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_fndSelect[5] , SELECT_OFF );
	}
	
	return 0; 
}

static void release_perifnd_gpio(struct perifnd_data *pData)
{
	int i ;
	for(i = 0; i < FND_LED_GPIO_NUM ; i++)
	{
		gpio_free(pData->gpio_fndled[i]);
	}

	for(i = 0; i < MAX_FND_NUM ; i++)
	{
		gpio_free(pData->gpio_fndSelect[i]);
	}

	printk("enter %s ---\n", __func__); 
	
}  
#define MS_TO_NS(x) (x * 1E6L)
static int perifnd_probe(struct platform_device *pdev)
{
	int retval;
	//struct device *temp_class;
	struct device_node *np;

	struct perifnd_data *pData;
	ktime_t ktime;

	retval = 0;

	np = pdev->dev.of_node;
	printk("%s enter+++\n",__func__); 
	if (!np)
		return -ENODEV;

	pData = (struct perifnd_data *)kmalloc(sizeof(struct perifnd_data), GFP_KERNEL);
	printk("pData:0x%p\n", pData); 
	if (pData) {
		mutex_init(&pData->lock);
		pData->gpio_fndled[0] = of_get_named_gpio(np, "perifnd-gpio-a", 0);
		pData->gpio_fndled[1] = of_get_named_gpio(np, "perifnd-gpio-b", 0);
		pData->gpio_fndled[2] = of_get_named_gpio(np, "perifnd-gpio-c", 0);
		pData->gpio_fndled[3] = of_get_named_gpio(np, "perifnd-gpio-d", 0);
		pData->gpio_fndled[4] = of_get_named_gpio(np, "perifnd-gpio-e", 0);
		pData->gpio_fndled[5] = of_get_named_gpio(np, "perifnd-gpio-f", 0);
		pData->gpio_fndled[6] = of_get_named_gpio(np, "perifnd-gpio-g", 0);
		pData->gpio_fndled[7] = of_get_named_gpio(np, "perifnd-gpio-dp", 0);
		
		pData->gpio_fndSelect[0] = of_get_named_gpio(np, "perifnd-gpio-s0", 0);
		pData->gpio_fndSelect[1] = of_get_named_gpio(np, "perifnd-gpio-s1", 0);
		pData->gpio_fndSelect[2] = of_get_named_gpio(np, "perifnd-gpio-s2", 0);
		pData->gpio_fndSelect[3] = of_get_named_gpio(np, "perifnd-gpio-s3", 0);
		pData->gpio_fndSelect[4] = of_get_named_gpio(np, "perifnd-gpio-s4", 0);
		pData->gpio_fndSelect[5] = of_get_named_gpio(np, "perifnd-gpio-s5", 0);
		
	    
		pData->pinctrl = devm_pinctrl_get(&pdev->dev);
		pData->pins_default = pinctrl_lookup_state(pData->pinctrl, PINCTRL_DEFAULT);
	} else {
		printk("Failed to allocate data structure.");
		retval = -ENOMEM;
		goto ERROR_1;
	}

	pinctrl_select_state(pData->pinctrl, pData->pins_default);
	release_perifnd_gpio(pData);


	if ( setup_perifnd_gpio(pData) )
	{
		printk("Failed to allocate gpio setup.");
		retval = -1;
		goto ERROR_2;
	}
	
	perifnd_initVariable(pData);
	
	dev_set_drvdata(&pdev->dev, pData);

	perifnd_miscdev.parent = &pdev->dev;

	
	printk("perifnd_miscdev.parent:%p\n", perifnd_miscdev.parent);
	retval = misc_register(&perifnd_miscdev);
	if (retval) 
	{
		printk("Failed to register the miscdevice \n");
		goto ERROR_2;
	}
	printk("%s enter+++ 2\n",__func__); 

	pPerifndData = pData;
	
	// add timer   
	ktime = ktime_set( 2,0);

	hrtimer_init( &pData->display_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
	pData->display_timer.function = &perifnd_periodic_work;
	printk("hrtimer_start------\n");
	hrtimer_start( &pData->display_timer, ktime, HRTIMER_MODE_REL );

	return 0;

ERROR_2:
	mutex_destroy(&pData->lock);
	kfree(pData);
ERROR_1:

	return  retval;
}

static int perifnd_remove(struct platform_device *pdev)
{
	struct perifnd_data *pData;
	int i;
	pData = dev_get_drvdata(&pdev->dev);	
	// deselect all 
	for(i = 0; i < 6 ; i++)
	{
		gpio_set_value(pData->gpio_fndSelect[i], SELECT_OFF);
	}
	
	release_perifnd_gpio(pData);

	hrtimer_cancel( &pData->display_timer );
	mutex_destroy(&pData->lock);
	
	kfree( pData);
	pPerifndData = NULL;
	misc_deregister(&perifnd_miscdev);
	
	return 0;
}

static struct platform_driver perifnd_driver = {
	.driver = {
		.name   = DRIVER_NAME,
		.owner  = THIS_MODULE,
		.of_match_table = perifnd_dt_ids,
	},
	.id_table = perifnd_devtype,
	.probe  = perifnd_probe,
	.remove = perifnd_remove,
};

module_platform_driver(perifnd_driver);

MODULE_ALIAS("platform:"DRIVER_NAME);
MODULE_LICENSE("GPL");

