/**
 * @file fake_muse.h
 * @author Frederic Simard (frederic.simard.1@outlook.com)
 * @brief Header for fake muse hardware 
 */

int fake_muse_connect_dev(void *param);
int fake_muse_init_hardware(void *param);
int fake_muse_read_pkt(void *param);
int fake_muse_send_keep_alive_pkt(void *param);
int fake_muse_send_pkt(void *param);
int fake_muse_translate_pkt(void *param);
int fake_muse_process_pkt(void * param);
int fake_muse_cleanup(void *param);
