#ifndef CEREBRALWARS_LIB_H
#define CEREBRALWARS_LIB_H
/**
 * @file cerebralwars_lib.h
 * @author Frederic Simard (frederic.simard.1@outlook.com) | atlants embedded
 * @brief 
 */
 
/*init function*/
void setup_cerebralwars(void);

/*user led interface*/
void set_training_mode(void);
void set_pairing_mode(void);
void set_game_mode(void);
void set_gameover_mode(void);
void reset_mode(void);

/*led strip functions*/
void compute_ledstrip_newstate(int explosion_loc, int* player1_particules_loc, int player1_nb_particules, int* player2_particules_loc, int player2_nb_particules);
void push_ledstrip_state(void);

/*clean up*/
void cleanup_cerebralwars(void);

#endif
