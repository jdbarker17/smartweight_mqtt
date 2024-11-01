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

void read_IMU(struct sensor_value acc[3], struct sensor_value gyr[3])
{
	const struct device *const bmi270Dev = DEVICE_DT_GET_ONE(bosch_bmi270);
    //struct sensor_value acc[3], gyr[3];
	struct sensor_value full_scale, sampling_freq, oversampling;

	if (!device_is_ready(bmi270Dev)) 
	{
		printf("Device %s is not ready\n", bmi270Dev->name);
		return acc;
	}
    
	LOG_INF("Device %p name is %s\n", bmi270Dev, bmi270Dev->name);

    /* Setting scale in G, due to loss of precision if the SI unit m/s^2
	 * is used
	 */
	full_scale.val1 = 2;            /* G */ //Accelerometers will use G, Gyros will use Deg/second
	full_scale.val2 = 0;
	sampling_freq.val1 = 100;       /* Hz. Performance mode */
	sampling_freq.val2 = 0;
	oversampling.val1 = 1;          /* Normal mode */
	oversampling.val2 = 0;

    sensor_attr_set(bmi270Dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_FULL_SCALE,
			&full_scale);
	sensor_attr_set(bmi270Dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_OVERSAMPLING,
			&oversampling);
    sensor_attr_set(bmi270Dev, SENSOR_CHAN_ACCEL_XYZ,
			SENSOR_ATTR_SAMPLING_FREQUENCY,
			&sampling_freq);

    /* Setting scale in degrees/s to match the sensor scale */
	full_scale.val1 = 500;          /* dps */
	full_scale.val2 = 0;
	sampling_freq.val1 = 100;       /* Hz. Performance mode */
	sampling_freq.val2 = 0;
	oversampling.val1 = 1;          /* Normal mode */
	oversampling.val2 = 0;

	sensor_attr_set(bmi270Dev, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_FULL_SCALE,
			&full_scale);
	sensor_attr_set(bmi270Dev, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_OVERSAMPLING,
			&oversampling);
	
	sensor_attr_set(bmi270Dev, SENSOR_CHAN_GYRO_XYZ,
			SENSOR_ATTR_SAMPLING_FREQUENCY,
			&sampling_freq);


    
    // 10ms period, 100Hz Sampling frequency 
    k_sleep(K_MSEC(100));

    sensor_sample_fetch(bmi270Dev);
    sensor_channel_get(bmi270Dev, SENSOR_CHAN_ACCEL_XYZ, acc);
    sensor_channel_get(bmi270Dev, SENSOR_CHAN_GYRO_XYZ, gyr);

    LOG_INF("Accelerometer Values are: AX: %d.%06d; AY: %d.%06d; AZ: %d.%06d;", 
            acc[0].val1, acc[0].val2,
            acc[1].val1, acc[1].val2,
            acc[2].val1, acc[2].val2);
    LOG_INF("Gyro Values are: GX: %d.%06d; GY: %d.%06d; GZ: %d.%06d;", 
            gyr[0].val1, gyr[0].val2,
            gyr[1].val1, gyr[1].val2,
            gyr[2].val1, gyr[2].val2);
            

    return acc;
}