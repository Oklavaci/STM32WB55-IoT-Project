/**
  ******************************************************************************
  * @file    README.txt
  * @author  Suleyman Çıtır
  * @brief   CAPSULE_00 8 bit 256 byte packaged data transfer with notify characteristics

  ******************************************************************************
  * @attention

  	  READ AN5289

	1-) We defined a Service in "STM32_WPAN/App" with a suffix "_app.c", who only publicly
	expose a void "P2PS_APP_Init()" function. This function defines behavior;
	task registers with "UTIL_SEQ_RegTask". This registered function does what is
	inside and updates the value for a characteristic using "aci_gatt_update_char_value",
	which automatically will send data for notifications/indications that are enabled.

	2-) These service Init() function are called from "APP_BLE_Init()" in "STM32_WPAN/app_ble.c".

	3-) In main, our interface to the BLE peripheral is managed with APPE_Init() in tandem with
	the sequencer, which runs with a default parameters that force it to consider
	all registered tasks.  "MX_APPE_Init()" is declared in "Core/app_entry.c", where
	it initializes the timer server and power modes.

	4-) "APP_BLE_Init()" from (2) above is called from "APPE_SysUserEvtRx()" in "app_entry.c";
	"APPE_SysUserEvtRx()" is called when CPU2 sends a ready signal that comes from
	this "APPE_Init()" process initializing CPU2 in "app_entry.c".

	5-) APPE_Init() also calls APPD_Init() in app_debug.c, which sets up either
	HAL managed debugging or exposes debugging traces on the GPIO pins.

	6-) Notification is pushed by the peripheral to the device
	asynchronously when a button is pressed on the peripheral in this version.

	7-) To change the names of services go to STM32_WPAN/App/app_ble.c find "a_LocalName"
	and change it btw this is local_name, name is can be changed from "*name=".

	8-) To increase the data size to be send change "CFG_BLE_MAX_ATT_MTU" in "app_conf.h" 
	its default value is 156 and allows 23 bytes and 3 bytes are reserved 
	so 20 byte of data can be send.

	9-) "aci_gatt_update_char_value" in "p2p_stm.c" updates the the characteristic that is
	already registered with a certain packet size. So use it.

	10-) In "p2p_stm.c" file I have created "P2PS_STM_App_Update_Int16" function
	which is the core function of the app. Declare created function in the
	appropriate header "in p2p_stm.h".

	11-) Edit the declaration of the characteristic so it's set up to recieve 16
	(or whatever bytes determined by MTU size) in "p2p_stm.c". Simply change the
	"Char_Value_Length" from 2 to 16 or whatever you set.

	12-) Edit button push or another interrupt in P2PS_Send_Notification from
	"p2p_server_app.c" to send a packet

  ******************************************************************************
  */