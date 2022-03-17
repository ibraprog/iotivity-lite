#include <pthread.h>
#include <signal.h>
#include <stdlib.h>

#include "ausy_gateway.h"
#include "oc_log.h"
#include "oc_api.h"

//enum client_context AUSY_CLIENT_CONTEXT = OCF;
//Gateway *AUSY_GATEWAY = NULL;

static pthread_mutex_t mutex;
static pthread_cond_t cv;
static struct timespec ts;
static int quit = 0;

static int
app_init(void)
{
    int ret = oc_init_platform("Ikea", NULL, NULL);
    ret |= oc_add_device("/oic/d", "oic.d.bridge", "Ikea bridge platform","ocf.2.2.5",
                         "ocf.res.1.3.0", NULL, NULL);

    return ret;
}

static bool
get_ocf_equivalence_from_zigbee_device(int zigbee_device_id, char* ocf_mrt,
                                                   char *ocf_rt, char *ocf_name)
{
    bool is_mapped = true;
    if (zigbee_device_id == 0x0002 || zigbee_device_id == 0x0009 || zigbee_device_id == 0x0051) {
        ocf_mrt = "oic.r.switch.binary";
        ocf_rt = "oic.d.smartplug";
        ocf_name = "Smart Plug";
    } else if (zigbee_device_id == 0x0100 || zigbee_device_id == 0x0101 || zigbee_device_id == 0x0102
            || zigbee_device_id == 0x010c || zigbee_device_id == 0x010d) {
        ocf_mrt = "oic.r.switch.binary";
        ocf_rt = "oic.d.light";
        ocf_name = "Light";
    } else if (zigbee_device_id == 0x0107) {
        ocf_mrt = "oic.r.sensor.presence";
        ocf_rt = "oic.d.sensor";
        ocf_name = "Generic Sensor";
    } else if (zigbee_device_id == 0x0202) {
        ocf_mrt = "oic.r.openlevel";
        ocf_rt = "oic.d.blind";
        ocf_name = "Blind";
    } else if (zigbee_device_id == 0x0301) {
        ocf_mrt = "oic.r.temperature";
        ocf_rt = "oic.d.thermostat";
        ocf_name = "Thermostat";
    } else if (zigbee_device_id == 0x0302) {
        ocf_mrt = "oic.r.temperature";
        ocf_rt = "oic.d.sensor";
        ocf_name = "Generic Sensor";
    } else if (zigbee_device_id == 0x0402) {
        ocf_mrt = "oic.r.ias.zone";
        ocf_rt = "oic.d.sensor";
        ocf_name = "Generic Sensor";
    } else {
        is_mapped = false;
    }

    return is_mapped;
}

static void
discovery()
{

}

static void
issue_requests(void)
{
  //oc_do_ip_discovery("oic.r.light", &discovery, NULL);
}

static void
signal_event_loop(void)
{
  pthread_mutex_lock(&mutex);
  pthread_cond_signal(&cv);
  pthread_mutex_unlock(&mutex);
}

static void
handle_signal(int signal)
{
  (void)signal;
  signal_event_loop();
  quit = 1;
}

int main()
{
    int init;
    struct sigaction sa;
    sigfillset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);

    /*AUSY_GATEWAY = (Gateway *) malloc(sizeof(Gateway));
    if (AUSY_GATEWAY == NULL) {
        OC_ERR("Memory allocation for AUSY_GATEWAY object failed!");
        return 0;
    }*/

    static const oc_handler_t handler = { .init = app_init,
                                          .signal_event_loop = signal_event_loop};

    oc_clock_time_t next_event;

    init = oc_main_init(&handler);
    if (init < 0)
      return init;

    while (quit != 1) {
      next_event = oc_main_poll();
      pthread_mutex_lock(&mutex);
      if (next_event == 0) {
        pthread_cond_wait(&cv, &mutex);
      } else {
        ts.tv_sec = (next_event / OC_CLOCK_SECOND);
        ts.tv_nsec = (next_event % OC_CLOCK_SECOND) * 1.e09 / OC_CLOCK_SECOND;
        pthread_cond_timedwait(&cv, &mutex, &ts);
      }
      pthread_mutex_unlock(&mutex);
    }

    oc_main_shutdown();

    //free(AUSY_GATEWAY);

    return 0;
}
