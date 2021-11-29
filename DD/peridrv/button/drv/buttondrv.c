#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/input.h>
#include <linux/input-polldev.h>
#include <asm/uaccess.h>


#define  LOW 		0
#define  HIGH		1

#define DRIVER_NAME              "peributton"
#define PINCTRL_DEFAULT          "default"

#define  KEY_INDEX_VOLUMEUP		0  // KEY_VOLUMEUP      115
#define  KEY_INDEX_HOME			1  // KEY_HOME          102
#define  KEY_INDEX_SEARCH		2  // KEY_SEARCH        217 	
#define  KEY_INDEX_BACK			3  // KEY_BACK          158
#define  KEY_INDEX_MENU			4  // KEY_MENU          139 
#define  KEY_INDEX_VOLUMEDOWN	5  // KEY_VOLUMEDOWN	114
#define  KEY_INDEX_MAX			6

static const unsigned int keyCodeConv[KEY_INDEX_MAX] = 
{
	KEY_VOLUMEUP,
	KEY_HOME,
	KEY_SEARCH,
	KEY_BACK,
	KEY_MENU,
	KEY_VOLUMEDOWN
};

struct peributton_data {
	struct pinctrl *pinctrl;	
	struct pinctrl_state *pins_default;
	struct mutex				lock;
	int	   polarity;
	int	   polling_interval;	//mSec
	int    gpio_button[KEY_INDEX_MAX];
};



static struct platform_device_id peributton_devtype[] = {
	{
		.name = DRIVER_NAME,
		.driver_data = 0,
	}, {
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(platform, peributton_devtype);

static const struct of_device_id peributton_dt_ids[] = {
	{ .compatible = "cndi,peributton", .data = &peributton_devtype[0], },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, peributton_dt_ids);

static void peributton_scan(struct input_polled_dev *dev)
{
	struct peributton_data *pData = dev->private;
	struct input_dev *input = dev->input;
	int i; 
	
	for( i = 0; i < KEY_INDEX_MAX; i++ )
	{
		input_report_key(input, keyCodeConv[i] , gpio_get_value(pData->gpio_button[i]) ^ pData->polarity );
	}
	input_sync(input);
}


static void peributton_initVariable(struct peributton_data *pData)
{
	pData->polling_interval =  4*8;
	pData->polarity = 1;
}

static int setup_peributton_gpio(struct peributton_data *pData)  
{
	int ret;
	ret = gpio_request(pData->gpio_button[KEY_INDEX_VOLUMEUP], "volumeup");
	if (ret) {
		printk(KERN_ERR "request peributton volumeup fail.\n");
		return -1;
	} else {
		gpio_direction_input(pData->gpio_button[KEY_INDEX_VOLUMEUP]);
	}
	
	ret = gpio_request(pData->gpio_button[KEY_INDEX_HOME], "home");
	if (ret) {
		printk(KERN_ERR "request peributton home fail.\n");
		return -1;
	} else {
		gpio_direction_input(pData->gpio_button[KEY_INDEX_HOME]);
	}

	ret = gpio_request(pData->gpio_button[KEY_INDEX_SEARCH], "search");
	if (ret) {
		printk(KERN_ERR "request peributton search fail.\n");
		return -1;
	} else {
		gpio_direction_input(pData->gpio_button[KEY_INDEX_SEARCH]);
	}
	
	ret = gpio_request(pData->gpio_button[KEY_INDEX_BACK], "back");
	if (ret) {
		printk(KERN_ERR "request peributton back fail.\n");
		return -1;
	} else {
		gpio_direction_input(pData->gpio_button[KEY_INDEX_BACK]);
	}
	
	ret = gpio_request(pData->gpio_button[KEY_INDEX_MENU], "menu");
	if (ret) {
		printk(KERN_ERR "request peributton menu fail.\n");
		return -1;
	} else {
		gpio_direction_input(pData->gpio_button[KEY_INDEX_MENU]);
	}
	
	ret = gpio_request(pData->gpio_button[KEY_INDEX_VOLUMEDOWN], "volumedown");
	if (ret) {
		printk(KERN_ERR "request peributton volumedown fail.\n");
		return -1;
	} else {
		gpio_direction_input(pData->gpio_button[KEY_INDEX_VOLUMEDOWN]);
	}
	return 0; 
}

static void release_peributton_gpio(struct peributton_data *pData)
{
	int i ;
	for(i = 0; i < KEY_INDEX_MAX ; i++)
	{
		gpio_free(pData->gpio_button[i]);
	}

}  


static int peributton_probe(struct platform_device *pdev)
{
	int retval;
	struct device_node *np;

	struct peributton_data *pData;
	struct input_polled_dev *input_poll;	
	struct input_dev *input;

	retval = 0;

	np = pdev->dev.of_node;
	printk("%s enter+++\n",__func__); 
	if (!np)
		return -ENODEV;

	pData = (struct peributton_data *)kmalloc(sizeof(struct peributton_data), GFP_KERNEL);
	
	printk("pData:0x%p\n", pData); 
	
	if (pData) {
		mutex_init(&pData->lock);
		pData->gpio_button[KEY_INDEX_VOLUMEUP] 	= 	of_get_named_gpio(np, "peributton-gpio-vol-up", 0);
		pData->gpio_button[KEY_INDEX_HOME] 		= 	of_get_named_gpio(np, "peributton-gpio-home", 0);
		pData->gpio_button[KEY_INDEX_SEARCH] 	= 	of_get_named_gpio(np, "peributton-gpio-search", 0);
		pData->gpio_button[KEY_INDEX_BACK] 		= 	of_get_named_gpio(np, "peributton-gpio-back", 0);
		pData->gpio_button[KEY_INDEX_MENU] 		= 	of_get_named_gpio(np, "peributton-gpio-menu", 0);
		pData->gpio_button[KEY_INDEX_VOLUMEDOWN]= 	of_get_named_gpio(np, "peributton-gpio-vol-down", 0);
		
	    
		pData->pinctrl = devm_pinctrl_get(&pdev->dev);
		pData->pins_default = pinctrl_lookup_state(pData->pinctrl, PINCTRL_DEFAULT);
	} else {
		printk("Failed to allocate data structure.");
		retval = -ENOMEM;
		goto ERROR_1;
	}

	pinctrl_select_state(pData->pinctrl, pData->pins_default);
	release_peributton_gpio(pData);

	if ( setup_peributton_gpio(pData) )
	{
		printk("Failed to allocate gpio setup.");
		retval = -1;
		goto ERROR_2;
	}
	
	peributton_initVariable(pData);
	
	input_poll = input_allocate_polled_device();
	if (!input_poll) {
		dev_err(&pdev->dev, "not enough memory for input device\n");
		retval = -ENOMEM;
		goto ERROR_3;
	}
	platform_set_drvdata(pdev, input_poll);

	/* set input-polldev handlers */
	input_poll->private = pData;
	input_poll->poll = peributton_scan;
	input_poll->poll_interval = pData->polling_interval;

	input = input_poll->input;
	input->name = "ecube-button";
	input->id.bustype = BUS_HOST;
	input->dev.parent = &pdev->dev;
	
	input_set_capability(input, EV_KEY, KEY_HOME);
	input_set_capability(input, EV_KEY, KEY_SEARCH);
	input_set_capability(input, EV_KEY, KEY_BACK);
	input_set_capability(input, EV_KEY, KEY_MENU);
	input_set_capability(input, EV_KEY, KEY_VOLUMEUP);
	input_set_capability(input, EV_KEY, KEY_VOLUMEDOWN);

	retval = input_register_polled_device(input_poll);
	if (retval) {
		dev_err(&pdev->dev, "could not register input device\n");
		goto ERROR_4;
	}
	return 0;
	
ERROR_4:
	input_free_polled_device(input_poll);
ERROR_3:
	release_peributton_gpio(pData);
ERROR_2:
	mutex_destroy(&pData->lock);
	kfree(pData);
ERROR_1:

	return  retval;
}

static int peributton_remove(struct platform_device *pdev)
{
	struct input_polled_dev *input_poll = platform_get_drvdata(pdev);
	struct peributton_data *pData = input_poll->private;

	release_peributton_gpio(pData);
	
	input_unregister_polled_device(input_poll);
	input_free_polled_device(input_poll);
	
	mutex_destroy(&pData->lock);
	
	kfree( pData);

	return 0;
}

static struct platform_driver peributton_driver = {
	.driver = {
		.name   = DRIVER_NAME,
		.owner  = THIS_MODULE,
		.of_match_table = peributton_dt_ids,
	},
	.id_table = peributton_devtype,
	.probe  = peributton_probe,
	.remove = peributton_remove,
};

module_platform_driver(peributton_driver);

MODULE_ALIAS("platform:"DRIVER_NAME);
MODULE_LICENSE("GPL");

