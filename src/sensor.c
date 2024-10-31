 /*
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <soc.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include <zephyr/settings/settings.h>

#include <dk_buttons_and_leds.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/adc.h>
#include <hal/nrf_saadc.h>
//#include "services/sensor_hub_service.h"

#include <zephyr/device.h>
#include <stdio.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(ACC_SENSOR, LOG_LEVEL_INF);

int read_IMU(void)
{
	const struct device *sensor = DEVICE_DT_GET_ANY(bmi270);

	if (!device_is_ready(sensor)) 
	{
		printk("Sensor device not ready\n");
		return;
	}
	LOG_INF("Hello World! %s\n", CONFIG_BOARD);

    return 0;
}