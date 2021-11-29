/*
 * udoo_ard.c
 * UDOO quad/dual Arduino flash erase / CPU resetter
 *
 * Copyright (C) 2013-2015 Aidilab srl
 * Author: UDOO Team <social@udoo.org>
 * Author: Giuseppe Pagano <giuseppe.pagano@seco.com>
 * Author: Francesco Montefoschi <francesco.monte@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#include <asm/uaccess.h>

#define DRIVER_NAME              "periled "
#define PINCTRL_DEFAULT          "default"

#define MAX_LED_NUM		8

struct periled_data{
	struct pinctrl *pinctrl;
	struct pinctrl_state *pins_default;
	int    cmdcode;
	unsigned int    gpio_led[MAX_LED_NUM];
} ;

//static int major; // for /dev/udoo_ard
//static struct class *udoo_class;

static struct platform_device_id periled_devtype[] = {
	{
		.name = DRIVER_NAME,
		.driver_data = 0,
	}, {
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(platform, periled_devtype);

static const struct of_device_id periled_dt_ids[] = {
	{ .compatible = "cndi,periled", .data = &periled_devtype[0], },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, periled_dt_ids);

static ssize_t periled_write(struct file *file, const char __user *data,
                             size_t len, loff_t *ppos);
static const struct file_operations periled_fops = {
	.write =                periled_write,
};

static struct miscdevice periled_miscdev = {
	.minor =        MISC_DYNAMIC_MINOR,
	.name =         "periled",
	.fops =         &periled_fops,
};

#define  LED_ON			1
#define  LED_OFF		0

static ssize_t periled_write(struct file *file, const char __user *data,
                             size_t len, loff_t *ppos)
{
	char msg[4];
	long res; 
	int i;

	int lenToCopy = 4;

//	struct miscdevice* md = file->private_data;
	struct miscdevice* md = &periled_miscdev;
	struct periled_data *pData ;

	printk("md:%p \n", md);
	printk("md->parent:%p \n",md->parent); 
	
	pData = dev_get_drvdata(md->parent);


	printk("enter %s +++ \n", __func__);

	printk("pData:0x%p\n", pData); 

	if ( len > 4)
		lenToCopy = 4;

	if (len == 0)
		return 0; 

	res = copy_from_user(msg, data, len);

	if (res)
	{
		return -EFAULT;
	}
        
	for(i = 0; i < MAX_LED_NUM ; i++ )
	{
		if ( msg[0] & (0x01 << i))
		{
			gpio_set_value(pData->gpio_led[i], LED_ON);
		}
		else
		{
			gpio_set_value(pData->gpio_led[i], LED_OFF);			
		}

	}

	return len;
}





static int setup_periled_gpio(struct periled_data *pData)  
{
	int ret;

	printk("pData->gpio_led[0]:%d\n",pData->gpio_led[0] ); 

	ret = gpio_request(pData->gpio_led[0], "periled 0");
	if (ret) {
		printk(KERN_ERR "request periled 0 fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_led[0],LED_OFF );
	}

	ret = gpio_request(pData->gpio_led[1], "periled 1");
	if (ret) {
		printk(KERN_ERR "request periled 1\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_led[1],LED_OFF );
	}
	ret = gpio_request(pData->gpio_led[2], "periled 2");
	if (ret) {
		printk(KERN_ERR "request periled 2\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_led[2],LED_OFF );
	}

	ret = gpio_request(pData->gpio_led[3], "periled 3");
	if (ret) {
		printk(KERN_ERR "request periled 3\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_led[3],LED_OFF );
	}

	ret = gpio_request(pData->gpio_led[4], "periled 4");
	if (ret) {
		printk(KERN_ERR "request periled 4\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_led[4],LED_OFF );
	}

	ret = gpio_request(pData->gpio_led[5], "periled 5");
	if (ret) {
		printk(KERN_ERR "request periled 5\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_led[5],LED_OFF );
	}

	ret = gpio_request(pData->gpio_led[6], "periled 6");
	if (ret) {
		printk(KERN_ERR "request periled 6\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_led[6],LED_OFF );
	}

	ret = gpio_request(pData->gpio_led[7], "periled 7");
	if (ret) {
		printk(KERN_ERR "request periled 7\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_led[7],LED_OFF );
	}

	return 0; 
}

static void release_periled_gpio(struct periled_data *pData)
{
	int i ;
	for(i = 0; i < MAX_LED_NUM ; i++)
	{
		gpio_free(pData->gpio_led[i]);
	}
	printk("enter %s ---\n", __func__); 
}  

static int periled_probe(struct platform_device *pdev)
{
	int retval;
	struct device_node *np;
	struct periled_data *pData;

	retval = 0;
	np = pdev->dev.of_node;
	printk("%s enter+++\n",__func__); 
	if (!np) return -ENODEV;

	pData = (struct periled_data *)kmalloc(sizeof(struct periled_data), GFP_KERNEL);
	printk("pData:0x%p\n", pData); 
	if (pData) {
		pData->gpio_led[0] = of_get_named_gpio(np, "periled-gpio-0", 0);
		printk("gpio_led 0 : %d\n", pData->gpio_led[0]);
		pData->gpio_led[1] = of_get_named_gpio(np, "periled-gpio-1", 0);
		printk("gpio_led 1 : %d\n", pData->gpio_led[1]);
		pData->gpio_led[2] = of_get_named_gpio(np, "periled-gpio-2", 0);
		pData->gpio_led[3] = of_get_named_gpio(np, "periled-gpio-3", 0);
		pData->gpio_led[4] = of_get_named_gpio(np, "periled-gpio-4", 0);
		pData->gpio_led[5] = of_get_named_gpio(np, "periled-gpio-5", 0);
		pData->gpio_led[6] = of_get_named_gpio(np, "periled-gpio-6", 0);
		pData->gpio_led[7] = of_get_named_gpio(np, "periled-gpio-7", 0);
		pData->pinctrl = devm_pinctrl_get(&pdev->dev);
		pData->pins_default = pinctrl_lookup_state(pData->pinctrl, PINCTRL_DEFAULT);
	} else {
		printk("Failed to allocate data structure.");
		retval = -ENOMEM;
		goto ERROR_1;
	}

	printk("%s enter+++ 1\n",__func__); 
	pinctrl_select_state(pData->pinctrl, pData->pins_default);
	release_periled_gpio(pData);


	if ( setup_periled_gpio(pData) )
	{
		printk("Failed to allocate gpio setup.");
		retval = -1;
		goto ERROR_2;
	}

	dev_set_drvdata(&pdev->dev, pData);

	periled_miscdev.parent = &pdev->dev;

	printk("periled_miscdev.parent:%p\n", periled_miscdev.parent);
	retval = misc_register(&periled_miscdev);
	if (retval) 
	{
		printk("Failed to register the miscdevice \n");
		goto ERROR_2;
	}
	printk("%s enter+++ 2\n",__func__); 

	return retval;
ERROR_2:
	kfree(pData);
ERROR_1:

	return  retval;
}

static int periled_remove(struct platform_device *pdev)
{
	struct periled_data *pData;
	pData = dev_get_drvdata(&pdev->dev);	
	release_periled_gpio(pData);

	kfree( pData);

	//device_destroy(udoo_class, MKDEV(major, 0));
	//class_destroy(udoo_class);
	//unregister_chrdev(major, "udoo_ard");
	misc_deregister(&periled_miscdev);

	return 0;
}

static struct platform_driver periled_driver = {
	.driver = {
		.name   = DRIVER_NAME,
		.owner  = THIS_MODULE,
		.of_match_table = periled_dt_ids,
	},
	.id_table = periled_devtype,
	.probe  = periled_probe,
	.remove = periled_remove,
};

module_platform_driver(periled_driver);

MODULE_ALIAS("platform:"DRIVER_NAME);
MODULE_LICENSE("GPL");
