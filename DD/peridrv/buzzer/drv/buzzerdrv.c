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
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <asm/uaccess.h>

// buzzer default state must be low. otherwise the current(A) is very high.
#define DRIVER_NAME              "peribuzzer"
#define PINCTRL_DEFAULT          "default"

#define LOW		0
#define HIGH	1

#define MAX_FREQUENCY	1000
#define MIN_FREQUENCY	1

struct peribuzzer_data {
	struct pinctrl *pinctrl;	
	struct pinctrl_state *pins_default;
	struct mutex				lock;
	struct hrtimer display_timer;
	int	   frequency;
	int    interval;  // nSec
	int	   enable;
	int	   currentGpioState;

	int    gpio_buzzer;
};

static struct platform_device_id peribuzzer_devtype[] = {
	{
		.name = DRIVER_NAME,
		.driver_data = 0,
	}, {
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(platform, peribuzzer_devtype);

static const struct of_device_id peribuzzer_dt_ids[] = {
	{ .compatible = "cndi,peribuzzer", .data = &peribuzzer_devtype[0], },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, peribuzzer_dt_ids);

static struct peribuzzer_data *pPeribuzzerData;
static enum hrtimer_restart peribuzzer_periodic_work(struct hrtimer *timer)
{
	ktime_t currtime , interval;

	if ( pPeribuzzerData != NULL)
	{
		currtime  = ktime_get();
		interval = ktime_set(0,pPeribuzzerData->interval); 
		if (pPeribuzzerData->enable )
		{
			if(pPeribuzzerData->currentGpioState)
			{
				pPeribuzzerData->currentGpioState = 0; 
				gpio_set_value(pPeribuzzerData->gpio_buzzer, LOW);
			}
			else
			{
				pPeribuzzerData->currentGpioState = 1; 
				gpio_set_value(pPeribuzzerData->gpio_buzzer, HIGH);
			}
		}
	}
	else
	{
		currtime  = ktime_get();
		interval = ktime_set(1,0); 
	}
  	hrtimer_forward(timer, currtime , interval);
//	printk("%s enter\n", __func__);
	return HRTIMER_RESTART;
}


static int setup_peribuzzer_gpio(struct peribuzzer_data *pData)  
{
	int ret;
	
	ret = gpio_request(pData->gpio_buzzer, "peribuzzer");
	if (ret) {
		printk(KERN_ERR "request peribuzzer fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_buzzer , LOW );
	}
	
	return 0; 
}

static void release_peribuzzer_gpio(struct peribuzzer_data *pData)
{
	gpio_free(pData->gpio_buzzer);
}  

static ssize_t peribuzzer_set_frequency(struct device *dev,
                struct device_attribute *attr, const char *buf, size_t count)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct peribuzzer_data *pData = platform_get_drvdata(pdev);
	long val;
	int err;
	err = strict_strtol(buf, 10, &val);
	if (err)
		return -EINVAL;

	// range 1 ~  1000;
	printk("frequency value:%d\n",(int)val);
	
	if ( (val > MAX_FREQUENCY) || (val < MIN_FREQUENCY))
	{
		printk("frequency range: 1 ~ 1000n");
		return -EINVAL;
	}
	pData->frequency = val; 
	pData->interval = 1000000000/ (pData->frequency * 2);  
	return count;
}

static ssize_t peribuzzer_get_frequency(struct device *dev,
                struct device_attribute *attr, char *buf)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct peribuzzer_data *pData = platform_get_drvdata(pdev);

    return sprintf(buf, "%d\n", pData->frequency);
}



static ssize_t peribuzzer_set_enable(struct device *dev,
                struct device_attribute *attr, const char *buf, size_t count)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct peribuzzer_data *pData = platform_get_drvdata(pdev);
	long val;
	int err;
	err = strict_strtol(buf, 10, &val);
	if (err)
		return -EINVAL;

	printk("enable value:%d\r\n",(int)val);
	if (val == 0) { // disable
		pData->enable = 0;
		gpio_set_value(pData->gpio_buzzer, LOW);  // no current
	} 
	else  // enable
	{
		pData->enable = 1;
	}
	return count;
}

static ssize_t peribuzzer_get_enable(struct device *dev,
                struct device_attribute *attr, char *buf)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct peribuzzer_data *pData = platform_get_drvdata(pdev);

    return sprintf(buf, "%d\n", pData->enable);
}

static DEVICE_ATTR(frequency, S_IWUSR | S_IRUGO,peribuzzer_get_frequency, peribuzzer_set_frequency);
static DEVICE_ATTR(enable, S_IWUSR | S_IRUGO,peribuzzer_get_enable,peribuzzer_set_enable );

static struct attribute *peribuzzer_attrs[] =  {
	&dev_attr_frequency.attr,
	&dev_attr_enable.attr,
	NULL
};
static const struct attribute_group peribuzzer_sysfs_files = {
	.attrs	= peribuzzer_attrs,
};

static int peribuzzer_probe(struct platform_device *pdev)
{
	int retval;
	//struct device *temp_class;
	struct device_node *np;

	struct peribuzzer_data *pData;
	ktime_t ktime;

	retval = 0;

	np = pdev->dev.of_node;
	printk("%s enter+++\n",__func__); 
	if (!np)
		return -ENODEV;

	pData = (struct peribuzzer_data *)kmalloc(sizeof(struct peribuzzer_data), GFP_KERNEL);
	printk("pData:0x%p\n", pData); 
	if (pData) {
		mutex_init(&pData->lock);
		pData->gpio_buzzer = of_get_named_gpio(np, "peribuzzer-gpio", 0);

		pData->pinctrl = devm_pinctrl_get(&pdev->dev);
		pData->pins_default = pinctrl_lookup_state(pData->pinctrl, PINCTRL_DEFAULT);
	} else {
		printk("Failed to allocate data structure.\n");
		retval = -ENOMEM;
		goto ERROR_1;
	}

	pinctrl_select_state(pData->pinctrl, pData->pins_default);
	release_peribuzzer_gpio(pData);


	if ( setup_peribuzzer_gpio(pData) )
	{
		printk("Failed to allocate gpio setup.");
		retval = -1;
		goto ERROR_2;
	}
	pData->enable = 0;
	pData->currentGpioState = 0;
	pData->frequency =  1;
	pData->interval = 1000000000/ ( pData->frequency * 2);  
	
	if(sysfs_create_group(&pdev->dev.kobj, &peribuzzer_sysfs_files))
	{
		printk("Failed sysfs_create_group.\n");
		retval = -1;
		goto ERROR_2;
	}
	dev_set_drvdata(&pdev->dev, pData);
	pPeribuzzerData = pData;
	// add timer   
	ktime = ktime_set( 2,0);

	hrtimer_init( &pData->display_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
	pData->display_timer.function = &peribuzzer_periodic_work;
	printk("hrtimer_start------\n");
	hrtimer_start( &pData->display_timer, ktime, HRTIMER_MODE_REL );

	return 0;

ERROR_2:
	mutex_destroy(&pData->lock);
	kfree(pData);
ERROR_1:

	return  retval;
}

static int peribuzzer_remove(struct platform_device *pdev)
{
	struct peribuzzer_data *pData;
	pData = dev_get_drvdata(&pdev->dev);	
	gpio_set_value(pData->gpio_buzzer, LOW);
	
	release_peribuzzer_gpio(pData);

	hrtimer_cancel( &pData->display_timer );
	mutex_destroy(&pData->lock);
	sysfs_remove_group(&pdev->dev.kobj , &peribuzzer_sysfs_files);
	pPeribuzzerData = NULL;
	kfree( pData);
	return 0;
}

static struct platform_driver peribuzzer_driver = {
	.driver = {
		.name   = DRIVER_NAME,
		.owner  = THIS_MODULE,
		.of_match_table = peribuzzer_dt_ids,
	},
	.id_table = peribuzzer_devtype,
	.probe  = peribuzzer_probe,
	.remove = peribuzzer_remove,
};

module_platform_driver(peribuzzer_driver);

MODULE_ALIAS("platform:"DRIVER_NAME);
MODULE_LICENSE("GPL");

