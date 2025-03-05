#ifndef _BEEP_H_
#define _BEEP_H_

enum beep_type {
    BEEP_PLAY = 1,
    BEEP_DELAY,
    BEEP_FREQ,
    BEEP_LENGTH,
};

struct beep_table {
    enum beep_type type;
    unsigned short value;
};

extern int
buzzer_init(void);

extern int
buzzer_beep(const struct beep_table *table);

#endif /* _BEEP_H_ */
