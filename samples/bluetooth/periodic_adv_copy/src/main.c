/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <bluetooth/bluetooth.h>

// use lowest interval possible (6ms)
static struct bt_le_per_adv_param per_adv_param = {
	.interval_min = 0x0006,
	.interval_max = 0x0006,
	.options = BT_LE_ADV_OPT_USE_TX_POWER | BT_LE_ADV_OPT_EXT_ADV,
};

static uint8_t mfg_data[] = { 0xff, 0xff, 0x00 };
static uint8_t test_data[14] = {0xff, 0x00, 0x12, 0x13, 0xff, 0xff, 0x00, 0x12, 0x13, 0xff,
								0xff, 0x00, 0x13, 0x13};

// use 240 byte payload for testing
static const struct bt_data ad[] = {
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, test_data, 14),
};

static void adv_sent_cb(struct bt_le_ext_adv *adv,
			struct bt_le_ext_adv_sent_info *info)
{
	printk("Advertiser[%d] %p sent %d\n", bt_le_ext_adv_get_index(adv),
	       adv, info->num_sent);
}

static struct bt_le_ext_adv_cb adv_callbacks = {
	.sent = adv_sent_cb,
};

void main(void)
{
	struct bt_le_ext_adv *adv;
	int err;

	printk("Starting Periodic Advertising Demo\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	/* Create a non-connectable non-scannable advertising set */
	err = bt_le_ext_adv_create(BT_LE_EXT_ADV_NCONN_NAME, &adv_callbacks, &adv);
	if (err) {
		printk("Failed to create advertising set (err %d)\n", err);
		return;
	}

	/* Set periodic advertising parameters */
	err = bt_le_per_adv_set_param(adv, &per_adv_param);
	if (err) {
		printk("Failed to set periodic advertising parameters"
		       " (err %d)\n", err);
		return;
	}

	/* Enable Periodic Advertising */
	err = bt_le_per_adv_start(adv);
	if (err) {
		printk("Failed to enable periodic advertising (err %d)\n", err);
		return;
	}

	/* Start extended advertising */
	err = bt_le_ext_adv_start(adv, BT_LE_EXT_ADV_START_DEFAULT);
	if (err) {
		printk("Failed to start extended advertising (err %d)\n", err);
		return;
	}


	while (true) {
		k_sleep(K_SECONDS(1));

		printk("Set Periodic Advertising Data...");
        err = bt_le_per_adv_set_data(adv, ad, ARRAY_SIZE(ad));
		if (err) {
			printk("Failed (err %d)\n", err);
			return;
		}
		printk("done.\n");
	}
}
