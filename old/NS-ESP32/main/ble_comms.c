#include "ble_comms.h"

static int manufacturer_name(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD.", strlen("ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD."));
    return 0;
}

static int firmware_revision(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "V0.1", strlen("V0.1"));
    return 0;
}

static int hardware_revision(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "ESP32 REV1", strlen("ESP32 REV1"));
    return 0;
}

static int software_revision(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "ESP-IDF V4.0.1", strlen("ESP-IDF V4.0.1"));
    return 0;
}

static int channel_number(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[8];
    sprintf(buffer, "%d", CHANNEL_NUM);
    os_mbuf_append(ctxt->om,&buffer[0],strlen(buffer));
    return 0;
}

static int maximum_frequency(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[8];
    sprintf(buffer, "%d", MAX_FREQ);
    os_mbuf_append(ctxt->om, &buffer[0], strlen(buffer));
    return 0;
}

static int ota_support(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om,"No", strlen("No"));
    return 0;
}

static int battry_level(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    //battery_update();
    os_mbuf_append(ctxt->om, &BATTERY_LEVEL, sizeof(BATTERY_LEVEL));
    return 0;
}

static int set_phase_one(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting phase one time as ");
    char *buffer = calloc(32, sizeof(char));
    memcpy(buffer,ctxt->om->om_data,ctxt->om->om_len);
    PHASE_ONE_TIME = atoi(buffer);
    printf("%d us\n", PHASE_ONE_TIME);
    ble_gattc_notify_custom(conn_hdl, phase_one_char_attr_hdl, ctxt->om);
    free(buffer);
    return 0;
}

static int read_phase_one(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[32];
    snprintf(buffer,32,"%d",PHASE_ONE_TIME);
    os_mbuf_append(ctxt->om, buffer, strlen(buffer));
    return 0;
}

static int set_phase_two(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting phase two time as ");
    char *buffer = calloc(32, sizeof(char));
    memcpy(buffer, ctxt->om->om_data, ctxt->om->om_len);
    PHASE_TWO_TIME = atoi(buffer);
    printf("%d us\n", PHASE_TWO_TIME);
    ble_gattc_notify_custom(conn_hdl, phase_two_char_attr_hdl, ctxt->om);
    free(buffer);
    return 0;
}

static int read_phase_two(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[32];
    snprintf(buffer,32, "%d", PHASE_TWO_TIME);
    os_mbuf_append(ctxt->om, buffer, strlen(buffer));
    return 0;
}

static int set_stim_amp(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting stim amp as ");
    char *buffer = calloc(32, sizeof(char));
    memcpy(buffer, ctxt->om->om_data, ctxt->om->om_len);
    STIM_AMP = atoi(buffer);
    printf("%d uA\n", STIM_AMP);
    ble_gattc_notify_custom(conn_hdl, stim_amp_char_attr_hdl, ctxt->om);
    free(buffer);
    return 0;
}

static int read_stim_amp(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[32];
    snprintf(buffer, 32, "%d", STIM_AMP);
    os_mbuf_append(ctxt->om, buffer, strlen(buffer));
    return 0;
}

static int set_inter_phase(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting interphase delay as ");
    char *buffer = calloc(32, sizeof(char));
    memcpy(buffer, ctxt->om->om_data, ctxt->om->om_len);
    INTER_PHASE_GAP = atoi(buffer);
    printf("%d us\n", INTER_PHASE_GAP);
    ble_gattc_notify_custom(conn_hdl, inter_phase_gap_char_attr_hdl, ctxt->om);
    free(buffer);
    return 0;
}

static int read_inter_phase(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[32];
    snprintf(buffer, 32, "%d", INTER_PHASE_GAP);
    os_mbuf_append(ctxt->om, buffer, strlen(buffer));
    return 0;
}

static int set_inter_stim(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting interstim delay as ");
    char *buffer = calloc(32, sizeof(char));
    memcpy(buffer, ctxt->om->om_data, ctxt->om->om_len);
    INTER_STIM_DELAY = atoi(buffer);
    printf("%d us\n", INTER_STIM_DELAY);
    ble_gattc_notify_custom(conn_hdl, inter_stim_delay_char_attr_hdl, ctxt->om);
    free(buffer);
    return 0;
}

static int read_inter_stim(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[32];
    snprintf(buffer, 32, "%d", INTER_STIM_DELAY);
    os_mbuf_append(ctxt->om, buffer, strlen(buffer));
    return 0;
}

static int set_pulse_num(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting stim duration as ");
    char *buffer = calloc(32, sizeof(char));
    memcpy(buffer, ctxt->om->om_data, ctxt->om->om_len);
    PULSE_NUM = atoi(buffer);
    printf("%d ms\n", PULSE_NUM);
    ble_gattc_notify_custom(conn_hdl, pulse_num_char_attr_hdl, ctxt->om);
    free(buffer);
    return 0;
}

static int read_pulse_num(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[32];
    snprintf(buffer, 32, "%d", PULSE_NUM);
    os_mbuf_append(ctxt->om, buffer, strlen(buffer));
    return 0;
}

static int set_acf(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting ");
    char *buffer = calloc(32, sizeof(char));
    memcpy(buffer, ctxt->om->om_data, ctxt->om->om_len);
    ANODIC_CATHODIC = atoi(buffer);
    printf("%s \n", ANODIC_CATHODIC?"CATHODIC":"ANODIC");
    ble_gattc_notify_custom(conn_hdl, anodic_cathodic_char_attr_hdl, ctxt->om);
    free(buffer);
    return 0;
}

static int read_acf(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[32];
    snprintf(buffer, 32, "%d", ANODIC_CATHODIC);
    os_mbuf_append(ctxt->om, buffer, strlen(buffer));
    return 0;
}

static int set_stim_type(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting stim type as ");
    char *buffer = calloc(32, sizeof(char));
    memcpy(buffer, ctxt->om->om_data, ctxt->om->om_len);
    STIM_TYPE = atoi(buffer);
    printf("%s\n", STIM_TYPE?"BURST":"UNIFORM");
    ble_gattc_notify_custom(conn_hdl, STIM_TYPE, ctxt->om);
    free(buffer);
    return 0;
}

static int read_stim_type(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[32];
    snprintf(buffer, 32, "%d", STIM_TYPE);
    os_mbuf_append(ctxt->om, buffer, strlen(buffer));
    return 0;
}

static int set_burst_num(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting burst number as ");
    char *buffer = calloc(32, sizeof(char));
    memcpy(buffer, ctxt->om->om_data, ctxt->om->om_len);
    BURST_NUM = atoi(buffer);
    printf("%d times\n", BURST_NUM);
    ble_gattc_notify_custom(conn_hdl, burst_num_char_attr_hdl, ctxt->om);
    free(buffer);
    return 0;
}

static int read_burst_num(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[32];
    snprintf(buffer, 32, "%d", BURST_NUM);
    os_mbuf_append(ctxt->om, buffer, strlen(buffer));
    return 0;
}

static int set_inter_burst(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting inter burst delay as ");
    char *buffer = calloc(32, sizeof(char));
    memcpy(buffer, ctxt->om->om_data, ctxt->om->om_len);
    INTER_BURST_DELAY = atoi(buffer);
    printf("%d us\n", INTER_BURST_DELAY);
    ble_gattc_notify_custom(conn_hdl, inter_burst_delay_char_attr_hdl, ctxt->om);
    free(buffer);
    return 0;
}

static int read_inter_burst(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[32];
    snprintf(buffer, 32, "%d", INTER_BURST_DELAY);
    os_mbuf_append(ctxt->om, buffer, strlen(buffer));
    return 0;
}

static int set_pulse_num_in_burst(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting pulse num in one burst as ");
    char *buffer = calloc(32, sizeof(char));
    memcpy(buffer, ctxt->om->om_data, ctxt->om->om_len);
    PULSE_NUM_IN_ONE_BURST = atoi(buffer);
    printf("%d\n", PULSE_NUM_IN_ONE_BURST);
    ble_gattc_notify_custom(conn_hdl, pulse_num_in_burst_char_attr_hdl, ctxt->om);
    free(buffer);
    return 0;
}

static int read_pulse_num_in_burst(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[32];
    snprintf(buffer, 32, "%d", PULSE_NUM_IN_ONE_BURST);
    os_mbuf_append(ctxt->om, buffer, strlen(buffer));
    return 0;
}

static int set_ramp_up(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char *buffer = calloc(32, sizeof(char));
    memcpy(buffer, ctxt->om->om_data, ctxt->om->om_len);
    RAMP_UP = atoi(buffer);
    ble_gattc_notify_custom(conn_hdl, ramp_up_char_attr_hdl, ctxt->om);
    free(buffer);
    return 0;
}

static int read_ramp_up(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[32];
    snprintf(buffer, 32, "%d", RAMP_UP);
    os_mbuf_append(ctxt->om, buffer, strlen(buffer));
    return 0;
}

static int set_short_electrode(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char *buffer = calloc(32, sizeof(char));
    memcpy(buffer, ctxt->om->om_data, ctxt->om->om_len);
    SHORT_ELECTRODE = atoi(buffer);
    ble_gattc_notify_custom(conn_hdl, short_electrode_char_attr_hdl, ctxt->om);
    free(buffer);
    return 0;
}

static int read_short_electrode(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[32];
    snprintf(buffer, 32, "%d", SHORT_ELECTRODE);
    os_mbuf_append(ctxt->om, buffer, strlen(buffer));
    return 0;
}

static int serial_set(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char *buffer = calloc(64,sizeof(char));
    memcpy(buffer, ctxt->om->om_data, ctxt->om->om_len);
    if (ctxt->om->om_len!=0){
        parse_command(buffer);
    }
    return 0;
}

/*
 * Add BLE services/characteristics/descriptors here
 */
static const struct ble_gatt_svc_def gatt_svcs[] = {
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = BLE_UUID16_DECLARE(DEVICE_INFO_SERVICE),
     .characteristics = (struct ble_gatt_chr_def[]){
         {.uuid = BLE_UUID16_DECLARE(MANUFACTURER_NAME_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = manufacturer_name},
         {.uuid = BLE_UUID16_DECLARE(FIRMWARE_REVISION_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = firmware_revision},
         {.uuid = BLE_UUID16_DECLARE(HARDWARE_REVISION_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = hardware_revision},
         {.uuid = BLE_UUID16_DECLARE(SOFTWARE_REVISION_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = software_revision},
         {.uuid = BLE_UUID128_DECLARE(CHANNEL_NUM_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = channel_number},
         {.uuid = BLE_UUID128_DECLARE(MAX_FREQ_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = maximum_frequency},
         {.uuid = BLE_UUID128_DECLARE(OTA_SUPPORT_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = ota_support},
         {0}}},
    {.type = BLE_GATT_SVC_TYPE_PRIMARY, 
     .uuid = BLE_UUID16_DECLARE(BATTRY_SERVICE), 
     .characteristics = (struct ble_gatt_chr_def[]){
         {.uuid = BLE_UUID16_DECLARE(BATTRY_LEVEL_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = battry_level, 
          .val_handle = &batt_char_attr_hdl},
         {0}}},
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = BLE_UUID128_DECLARE(STIMULATION_COMMAND_SERVICE),
     .characteristics = (struct ble_gatt_chr_def[]){
         {.uuid = BLE_UUID128_DECLARE(PHASE_ONE_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_phase_one},
         {.uuid = BLE_UUID128_DECLARE(PHASE_ONE_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_phase_one,
          .val_handle = &phase_one_char_attr_hdl},
         {.uuid = BLE_UUID128_DECLARE(PHASE_TWO_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_phase_two},
         {.uuid = BLE_UUID128_DECLARE(PHASE_TWO_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_phase_two,
          .val_handle = &phase_two_char_attr_hdl},
         {.uuid = BLE_UUID128_DECLARE(STIM_AMP_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_stim_amp},
         {.uuid = BLE_UUID128_DECLARE(STIM_AMP_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_stim_amp,
          .val_handle = &stim_amp_char_attr_hdl},
         {.uuid = BLE_UUID128_DECLARE(INTER_PHASE_GAP_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_inter_phase},
         {.uuid = BLE_UUID128_DECLARE(INTER_PHASE_GAP_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_inter_phase,
          .val_handle = &inter_phase_gap_char_attr_hdl},
         {.uuid = BLE_UUID128_DECLARE(INTER_STIM_DELAY_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_inter_stim},
         {.uuid = BLE_UUID128_DECLARE(INTER_STIM_DELAY_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_inter_stim,
          .val_handle = &inter_stim_delay_char_attr_hdl},
         {.uuid = BLE_UUID128_DECLARE(PULSE_NUM_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_pulse_num},
         {.uuid = BLE_UUID128_DECLARE(PULSE_NUM_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_pulse_num,
          .val_handle = &pulse_num_char_attr_hdl},
         {.uuid = BLE_UUID128_DECLARE(ANODIC_CATHODIC_FIRST_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_acf},
         {.uuid = BLE_UUID128_DECLARE(ANODIC_CATHODIC_FIRST_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_acf,
          .val_handle = &anodic_cathodic_char_attr_hdl},
         {.uuid = BLE_UUID128_DECLARE(STIM_TYPE_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_stim_type},
         {.uuid = BLE_UUID128_DECLARE(STIM_TYPE_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_stim_type,
          .val_handle = &stim_type_char_attr_hdl},
         {.uuid = BLE_UUID128_DECLARE(BURST_NUM_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_burst_num},
         {.uuid = BLE_UUID128_DECLARE(BURST_NUM_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_burst_num,
          .val_handle = &burst_num_char_attr_hdl},
         {.uuid = BLE_UUID128_DECLARE(INTER_BURST_DELAY_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_inter_burst},
         {.uuid = BLE_UUID128_DECLARE(INTER_BURST_DELAY_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_inter_burst,
          .val_handle = &inter_burst_delay_char_attr_hdl},
         {.uuid = BLE_UUID128_DECLARE(PULSE_NUM_IN_ONE_BURST_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_pulse_num_in_burst},
         {.uuid = BLE_UUID128_DECLARE(PULSE_NUM_IN_ONE_BURST_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_pulse_num_in_burst,
          .val_handle = &pulse_num_in_burst_char_attr_hdl},
         {.uuid = BLE_UUID128_DECLARE(RAMP_UP_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_ramp_up},
         {.uuid = BLE_UUID128_DECLARE(RAMP_UP_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_ramp_up,
          .val_handle = &ramp_up_char_attr_hdl},
         {.uuid = BLE_UUID128_DECLARE(SHORT_ELECTRODE_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_short_electrode},
         {.uuid = BLE_UUID128_DECLARE(SHORT_ELECTRODE_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_short_electrode,
          .val_handle = &short_electrode_char_attr_hdl},
         {.uuid = BLE_UUID128_DECLARE(SERIAL_COMMAND_INPUT_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = serial_set},
         {0}}},
    {0}};

static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type)
    {
        case BLE_GAP_EVENT_CONNECT:
            ESP_LOGI("GAP", "BLE_GAP_EVENT_CONNECT %s", event->connect.status == 0 ? "OK" : "Failed");
            if (event->connect.status != 0)
            {
                ble_app_advertise();
            }
            conn_hdl = event->connect.conn_handle;
            break;
        case BLE_GAP_EVENT_DISCONNECT:
            ESP_LOGI("GAP", "BLE_GAP_EVENT_DISCONNECT");
            ble_app_advertise();
            break;
        case BLE_GAP_EVENT_ADV_COMPLETE:
            ESP_LOGI("GAP", "BLE_GAP_EVENT_ADV_COMPLETE");
            ble_app_advertise();
            break;
        case BLE_GAP_EVENT_SUBSCRIBE:
            ESP_LOGI("GAP", "BLE_GAP_EVENT_SUBSCRIBE");
            break;
        default:
            break;
    }
    return 0;
}

void ble_app_advertise(void)
{
    struct ble_hs_adv_fields fields;
    memset(&fields, 0, sizeof(fields));

    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_DISC_LTD;
    fields.tx_pwr_lvl_is_present = 1;
    fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;

    fields.name = (uint8_t *)ble_svc_gap_device_name();
    fields.name_len = strlen(ble_svc_gap_device_name());
    fields.name_is_complete = 1;

    ble_gap_adv_set_fields(&fields);

    struct ble_gap_adv_params adv_params;
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL);
}

void ble_app_on_sync(void)
{
    ble_hs_id_infer_auto(0, &ble_addr_type);
    ble_app_advertise();
}

void host_task(void *param)
{
    nimble_port_run();
}

void ble_init(void)
{
    nvs_flash_init();

    esp_nimble_hci_and_controller_init();
    nimble_port_init();

    ble_svc_gap_device_name_set(DEVICE_NAME);
    ble_svc_gap_init();
    ble_svc_gatt_init();

    ble_gatts_count_cfg(gatt_svcs);
    ble_gatts_add_svcs(gatt_svcs);

    ble_hs_cfg.sync_cb = ble_app_on_sync;

    nimble_port_freertos_init(host_task);

    /* if(i2c_connection_status == 0){
        battery_update();
        battery_update_timer_handler = xTimerCreate("update_battery_timer", pdMS_TO_TICKS(BATTERY_UPDATE_TIME_INTERVAL), pdTRUE, NULL, battery_level_notify);
    }  */
}

void ble_deinit(){
    nimble_port_freertos_deinit();
    esp_nimble_hci_and_controller_deinit();
}

void battery_level_notify()
{
    battery_update();
    struct os_mbuf *om = ble_hs_mbuf_from_flat(&BATTERY_LEVEL, sizeof(BATTERY_LEVEL));
    ble_gattc_notify_custom(conn_hdl, batt_char_attr_hdl, om);
}
