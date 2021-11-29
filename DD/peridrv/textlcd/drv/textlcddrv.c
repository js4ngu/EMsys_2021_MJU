
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

#include "textlcddrv.h"

#define TRUE	1
#define FALSE	0

#define DRIVER_NAME              "peritextlcd"
#define PINCTRL_DEFAULT          "default"

#define CLEAR_DISPLAY		0x0001
#define CURSOR_AT_HOME		0x0002
// Entry Mode set 
#define MODE_SET_DEF		0x0004
#define MODE_SET_DIR_RIGHT	0x0002
#define MODE_SET_SHIFT		0x0001
// Display on off
#define DIS_DEF				0x0008
#define DIS_LCD				0x0004
#define DIS_CURSOR			0x0002
#define DIS_CUR_BLINK		0x0001
// shift
#define CUR_DIS_DEF			0x0010
#define CUR_DIS_SHIFT		0x0008
#define CUR_DIS_DIR			0x0004

#define LCD_E_ACTIVE	1   	 // action  falling edge
#define LCD_E_DEACTIVE	0		 

#define LCD_RW_READ		1
#define LCD_RW_WRITE	0

#define LCD_RS_DATA					1
#define LCD_RS_INSTRUCTION			0

#define BUSY_BIT			0x80

#define  TEXT_DATA_NUM		8

#define DDRAM_ADDR_LINE_1	0x00
#define DDRAM_ADDR_LINE_2	0x40

// set DDRAM  address 
#define SET_DDRAM_ADD_DEF	0x0080


struct peritextlcd_data{
	struct pinctrl *pinctrl;
	struct pinctrl_state *pins_default;
	unsigned int	gpio_lcddata[TEXT_DATA_NUM];
	unsigned int	gpio_rs;
	unsigned int	gpio_rw;
	unsigned int	gpio_e;	
} ;

static struct platform_device_id peritextlcd_devtype[] = {
	{
		.name = DRIVER_NAME,
		.driver_data = 0,
	}, {
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(platform, peritextlcd_devtype);

static const struct of_device_id peritextlcd_dt_ids[] = {
	{ .compatible = "cndi,peritextlcd", .data = &peritextlcd_devtype[0], },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, peritextlcd_dt_ids);

static ssize_t peritextlcd_write(struct file *file, const char __user *data,
                             size_t len, loff_t *ppos);
							 
static const struct file_operations peritextlcd_fops = {
	.write =                peritextlcd_write,
};

static struct miscdevice peritextlcd_miscdev = {
	.minor =        MISC_DYNAMIC_MINOR,
	.name =         "peritextlcd",
	.fops =         &peritextlcd_fops,
};

/***************************************************

RS,(R/W) => E (rise) => Data => E (fall) 
***************************************************/
static void Set_RS(struct peritextlcd_data *pData, int value)
{
	gpio_set_value(pData->gpio_rs, value);	
}

static void Set_E(struct peritextlcd_data *pData, int value)
{
	gpio_set_value(pData->gpio_e, value);
	mdelay(1);
}

static void Set_RW(struct peritextlcd_data *pData, int value)
{
	gpio_set_value(pData->gpio_rw, value);	
}

static void Set_Data(struct peritextlcd_data *pData, unsigned char value)
{
	int i; 
	for (i = 0; i < TEXT_DATA_NUM ; i++ )
	{
		if ( (value >> i) & 0x01 )
		{
			gpio_set_value(pData->gpio_lcddata[i], 1);	
		}
		else 
		{
			gpio_set_value(pData->gpio_lcddata[i], 0);	
		}
	}
}

static int IsBusy(struct peritextlcd_data *pData)
{
	
	unsigned char value;
	int i; 
	int busyCounter = 5; 
	
	while(busyCounter)
	{
		busyCounter--;
		for (i = 0; i < TEXT_DATA_NUM ; i++ )
		{
			gpio_direction_input(pData->gpio_lcddata[i]);
		}

		Set_RW(pData, LCD_RW_READ);
		
		Set_RS(pData, LCD_RS_INSTRUCTION);
		
		Set_E(pData, LCD_E_ACTIVE);
//		mdelay(1);
		value =0;
		for (i = 0; i < TEXT_DATA_NUM ; i++ )
		{
			if ( gpio_get_value(pData->gpio_lcddata[i]))
			{
				value |=(0x01 << i);
			}
		}

		Set_E(pData, LCD_E_DEACTIVE);
		
		Set_RW(pData, LCD_RW_WRITE);
		
		for (i = 0; i < TEXT_DATA_NUM ; i++ )
		{
			gpio_direction_output(pData->gpio_lcddata[i],1);
		}
		
		if ( BUSY_BIT & value)
		{
			if ( busyCounter == 0)
			{
				printk("%s busy counter error.\n",__func__);
				return TRUE;
			}			
			continue;
		}
		else
		{
			break;
		}
	}

	return FALSE;
	
}

static int writeCmd(struct peritextlcd_data *pData, unsigned char cmd)
{

	if ( IsBusy(pData))
	{
		printk("writeCmd busy error.\n");
		return FALSE;
	}

	Set_Data(pData, cmd);
	
	Set_RS(pData, LCD_RS_INSTRUCTION);
	
	Set_RW(pData, LCD_RW_WRITE);
	
	Set_E(pData, LCD_E_ACTIVE);
	
	// delay
//	mdelay(1);
	
	Set_E(pData, LCD_E_DEACTIVE);
	
	return TRUE;
}

static int setDDRAMAddr(struct peritextlcd_data *pData, int x , int y)
{
	unsigned char cmd = 0;
	if(IsBusy(pData))
	{
		printk("setDDRAMAddr busy error.\n");
		return FALSE;
	}

	if ( y == 1 )
	{
		cmd = DDRAM_ADDR_LINE_1 +x;
	}
	else if(y == 2 )
	{
		cmd = DDRAM_ADDR_LINE_2 +x;
	}
	else
		return FALSE;

	if ( cmd >= 0x80)
		return FALSE;

	if (!writeCmd(pData, cmd | SET_DDRAM_ADD_DEF))
	{
		printk("setDDRAMAddr error\n");
		return FALSE;
	}
	
	mdelay(1);
	
	return TRUE;
}

static int displayMode(struct peritextlcd_data *pData, int bCursor, int bCursorblink, int blcd  )
{
	unsigned char cmd  = 0;
	if ( bCursor)
	{
		cmd = DIS_CURSOR;
	}
	
	if (bCursorblink )
	{
		cmd |= DIS_CUR_BLINK;
	}
	
	if ( blcd )
	{
		cmd |= DIS_LCD;
	}
	
	if (!writeCmd(pData, cmd | DIS_DEF))
		return FALSE;
	
	mdelay(2);
	return TRUE;
}

static int writeCh(struct peritextlcd_data *pData, unsigned char ch)
{
	if ( IsBusy(pData))
	{
		printk("writeCh busy error.\n");
		return FALSE;
	}

	Set_Data(pData, ch);
	
	Set_RS(pData, LCD_RS_DATA);
	
	Set_RW(pData, LCD_RW_WRITE);
	
	Set_E(pData, LCD_E_ACTIVE);
	
	// delay
//	mdelay(1);
	Set_E(pData, LCD_E_DEACTIVE);	
	
	return TRUE;
}

static int setCursorMode(struct peritextlcd_data *pData, int bMove , int bRightDir)
{
	unsigned char cmd = MODE_SET_DEF;
	if (bMove)
		cmd |=  MODE_SET_SHIFT;

	if (bRightDir)
		cmd |= MODE_SET_DIR_RIGHT;

	if (!writeCmd(pData, cmd))
		return FALSE;

	mdelay(2);
	return TRUE;
}
static int functionSet(struct peritextlcd_data *pData)
{
	unsigned char cmd = 0x38; // 5*8 dot charater , 8bit interface , 2 line
	if (!writeCmd(pData, cmd))
		return FALSE;

	mdelay(1);

	return TRUE;
}
static int writeStr(struct peritextlcd_data *pData, char* str)
{
	unsigned char wdata;
	int i;
	for(i =0; i < strlen(str) ;i++ )
	{
		if (str[i] == '_')
			wdata = (unsigned char)' ';
		else
			wdata = str[i];
		writeCh(pData, wdata);
	}
	return TRUE;
}

		
static int clearScreen(struct peritextlcd_data *pData, int nline)
{
	int i;
	if (nline == 0)
	{
		if(IsBusy(pData))
		{	
			printk("clearScreen error\n");
			return FALSE;
		}
		if (!writeCmd(pData, CLEAR_DISPLAY))
			return FALSE;
		return TRUE;
	}
	else if (nline == 1)
	{	
		setDDRAMAddr(pData,0,1);
		for(i = 0; i <= COLUMN_NUM ;i++ )
		{
			writeCh(pData, (unsigned char)' ');
		}	
		setDDRAMAddr(pData,0,1);
	}
	else if (nline == 2)
	{	
		setDDRAMAddr(pData,0,2);
		for(i = 0; i <= COLUMN_NUM ;i++ )
		{
			writeCh(pData, (unsigned char)' ');
		}	
		setDDRAMAddr(pData,0,2);
	}
	mdelay(1);
	return TRUE;
}

static void peritextlcd_test(struct peritextlcd_data *pData, int lineNum , int gpioSetValue )
{
	// 0 7 => data line
	// 8 => RS
	// 9 => RW
	// 10 => E
	
	if ( (lineNum >10)  || (lineNum < 0 ) )
	{
		printk("%s lineNum over range:%d\n",__func__,lineNum);
		return; 
	}
	if ( lineNum < 8)
	{
		gpio_set_value(pData->gpio_lcddata[lineNum], gpioSetValue);	
		return;
	}
	
	if ( lineNum == 8)
	{
		gpio_set_value(pData->gpio_rs, gpioSetValue);	
		return;
	}
	if ( lineNum == 9)
	{
		gpio_set_value(pData->gpio_rw, gpioSetValue);	
		return;
	}
	if ( lineNum == 10)
	{
		gpio_set_value(pData->gpio_e, gpioSetValue);	
		return;
	}
}



static ssize_t peritextlcd_write(struct file *file, const char __user *data,
                             size_t len, loff_t *ppos)
{
	long res; 


	stTextLCD StText;
	//	struct miscdevice* md = file->private_data;
	struct miscdevice* md = &peritextlcd_miscdev;
	struct peritextlcd_data *pData ;
	int  moveEnable, rightDir;

	printk("md:%p \n", md);
	printk("md->parent:%p \n",md->parent); 
	
	pData = dev_get_drvdata(md->parent);
	
	printk("enter %s +++ \n", __func__);

	printk("pData:0x%p\n", pData);


	if ( len <  sizeof (stTextLCD))
		return 0;

	res = copy_from_user(&StText, data, sizeof (stTextLCD));

	if (res)
	{
		return -EFAULT;
	}
        
	switch ( StText.cmd)
	{
	case CMD_DISPLAY_MODE:
		displayMode(pData, BIT_DISPLAY_MODE_CURSOR_DISP & StText.cmdData,
			BIT_DISPLAY_MODE_CURSOR_BLINK & StText.cmdData, 
			BIT_DISPLAY_MODE_DISP_ENABLE & StText.cmdData );
	break;
	case CMD_CURSOR_MOVE_MODE:
		if ( CURSOR_MOVE_MODE_ENABLE & StText.cmdData)
			moveEnable = 1;
		else
			moveEnable = 0;
		if ( CURSOR_MOVE_MODE_RIGHT_DIR & StText.cmdData)
			rightDir = 1;
		else
			rightDir = 0;
		
		setCursorMode(pData, moveEnable , rightDir);
	break;
	case CMD_CURSOR_MOVE_POSITION:
		setDDRAMAddr(pData, CURSOR_MOVE_X_MASK & StText.cmdData ,(CURSOR_MOVE_Y_MASK & StText.cmdData) >> 6 );
	break;
	case CMD_WRITE_STRING:
		if (StText.cmdData == CMD_DATA_WRITE_LINE_1)
		{
			clearScreen(pData, 1);
			writeStr(pData, StText.TextData[0]);
		}
		else if( StText.cmdData == CMD_DATA_WRITE_LINE_2)
		{
			clearScreen(pData, 2);
			writeStr(pData, StText.TextData[1]);			
		}
		else // both
		{
			clearScreen(pData, 1);
			writeStr(pData, StText.TextData[0]);

			clearScreen(pData, 2);
			writeStr(pData, StText.TextData[1]);			
		}
	break;
	case CMD_TEST_GPIO_HIGH:
		peritextlcd_test(pData, StText.cmdData , 1 );
	break;
	case CMD_TEST_GPIO_LOW:
		peritextlcd_test(pData, StText.cmdData , 0 );
	break; 
	}

	return len;
}



static void peritextlcd_initcmd(struct peritextlcd_data *pData)
{

	functionSet(pData);
	displayMode(pData, 1,1,1 );	
	
	clearScreen(pData, 1);
	writeStr(pData, "lcd inited.");
	clearScreen(pData, 2);
	
	writeStr(pData, "ready...Ok");
	
}


static int setup_peritextlcd_gpio(struct peritextlcd_data *pData)  
{
	int ret;

	ret = gpio_request(pData->gpio_e, "peritextlcd enable");
	if (ret) {
		printk(KERN_ERR "request peritextlcd enable fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_e , LCD_E_DEACTIVE );
	}

	ret = gpio_request(pData->gpio_rw, "peritextlcd rw");
	if (ret) {
		printk(KERN_ERR "request peritextlcd rw fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_rw,LCD_RW_WRITE );
	}
	printk("%s pData->gpio_rw:%d\n ",__func__,pData->gpio_rw);
	printk("%s pData->gpio_rs:%d\n ",__func__,pData->gpio_rs);
	ret = gpio_request(pData->gpio_rs, "peritextlcd rs");
	if (ret) {
		printk(KERN_ERR "request peritextlcd rs fail.\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_rs,LCD_RS_INSTRUCTION );
	}

	ret = gpio_request(pData->gpio_lcddata[0], "peritextlcd 0");
	if (ret) {
		printk(KERN_ERR "request peritextlcd 0\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_lcddata[0],0 );
	}
	ret = gpio_request(pData->gpio_lcddata[1], "peritextlcd 1");
	if (ret) {
		printk(KERN_ERR "request peritextlcd 1\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_lcddata[1],0 );
	}
	ret = gpio_request(pData->gpio_lcddata[2], "peritextlcd 2");
	if (ret) {
		printk(KERN_ERR "request peritextlcd 2\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_lcddata[2],0 );
	}
	
	ret = gpio_request(pData->gpio_lcddata[3], "peritextlcd 3");
	if (ret) {
		printk(KERN_ERR "request peritextlcd 3\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_lcddata[3],0 );
	}

	ret = gpio_request(pData->gpio_lcddata[4], "peritextlcd 4");
	if (ret) {
		printk(KERN_ERR "request peritextlcd 4\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_lcddata[4],0 );
	}

	ret = gpio_request(pData->gpio_lcddata[5], "peritextlcd 5");
	if (ret) {
		printk(KERN_ERR "request peritextlcd 5\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_lcddata[5],0 );
	}

	ret = gpio_request(pData->gpio_lcddata[6], "peritextlcd 6");
	if (ret) {
		printk(KERN_ERR "request peritextlcd 6\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_lcddata[6],0 );
	}

	ret = gpio_request(pData->gpio_lcddata[7], "peritextlcd 7");
	if (ret) {
		printk(KERN_ERR "request peritextlcd 7\n");
		return -1;
	} else {
		gpio_direction_output(pData->gpio_lcddata[7],0 );
	}

	return 0; 
}

static void release_peritextlcd_gpio(struct peritextlcd_data *pData)
{
	int i ;
	for(i = 0; i < TEXT_DATA_NUM ; i++)
	{
		gpio_free(pData->gpio_lcddata[i]);
	}
	
	gpio_free(pData->gpio_rs);
	gpio_free(pData->gpio_rw);
	gpio_free(pData->gpio_e);
	
	printk("enter %s ---\n", __func__); 
	
}  

static int peritextlcd_probe(struct platform_device *pdev)
{
	int retval;
	//struct device *temp_class;
	struct device_node *np;

	struct peritextlcd_data *pData;

	retval = 0;

	np = pdev->dev.of_node;
	printk("%s enter+++\n",__func__); 
	if (!np)
		return -ENODEV;

	pData = (struct peritextlcd_data *)kmalloc(sizeof(struct peritextlcd_data), GFP_KERNEL);
	printk("pData:0x%p\n", pData); 
	if (pData) {
		pData->gpio_lcddata[0] = of_get_named_gpio(np, "perilcd-gpio-d0", 0);
		pData->gpio_lcddata[1] = of_get_named_gpio(np, "perilcd-gpio-d1", 0);
		pData->gpio_lcddata[2] = of_get_named_gpio(np, "perilcd-gpio-d2", 0);
		pData->gpio_lcddata[3] = of_get_named_gpio(np, "perilcd-gpio-d3", 0);
		pData->gpio_lcddata[4] = of_get_named_gpio(np, "perilcd-gpio-d4", 0);
		pData->gpio_lcddata[5] = of_get_named_gpio(np, "perilcd-gpio-d5", 0);
		pData->gpio_lcddata[6] = of_get_named_gpio(np, "perilcd-gpio-d6", 0);
		pData->gpio_lcddata[7] = of_get_named_gpio(np, "perilcd-gpio-d7", 0);
		
		pData->gpio_rs 		   = of_get_named_gpio(np, "perilcd-gpip-rs", 0);
		pData->gpio_rw 		   = of_get_named_gpio(np, "perilcd-gpio-rw", 0);
		pData->gpio_e 		   = of_get_named_gpio(np, "perilcd-gpio-e", 0);
		
		pData->pinctrl = devm_pinctrl_get(&pdev->dev);
		pData->pins_default = pinctrl_lookup_state(pData->pinctrl, PINCTRL_DEFAULT);
	} else {
		printk("Failed to allocate data structure.");
		retval = -ENOMEM;
		goto ERROR_1;
	}

	printk("%s enter+++ 1\n",__func__); 
	pinctrl_select_state(pData->pinctrl, pData->pins_default);
	release_peritextlcd_gpio(pData);


	if ( setup_peritextlcd_gpio(pData) )
	{
		printk("Failed to allocate gpio setup.");
		retval = -1;
		goto ERROR_2;
	}

	peritextlcd_initcmd(pData);
	
	
	dev_set_drvdata(&pdev->dev, pData);

	peritextlcd_miscdev.parent = &pdev->dev;

	
	printk("peritextlcd_miscdev.parent:%p\n", peritextlcd_miscdev.parent);
	retval = misc_register(&peritextlcd_miscdev);
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

static int peritextlcd_remove(struct platform_device *pdev)
{
	struct peritextlcd_data *pData;
	pData = dev_get_drvdata(&pdev->dev);	
	release_peritextlcd_gpio(pData);

	kfree( pData);

	misc_deregister(&peritextlcd_miscdev);

	return 0;
}

static struct platform_driver peritextlcd_driver = {
	.driver = {
		.name   = DRIVER_NAME,
		.owner  = THIS_MODULE,
		.of_match_table = peritextlcd_dt_ids,
	},
	.id_table = peritextlcd_devtype,
	.probe  = peritextlcd_probe,
	.remove = peritextlcd_remove,
};

module_platform_driver(peritextlcd_driver);

MODULE_ALIAS("platform:"DRIVER_NAME);
MODULE_LICENSE("GPL");
