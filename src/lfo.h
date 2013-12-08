#ifndef LFO_H
#define LFO_H

class Lfo
{
public:
    Lfo();

private:
    uint16_t& target;
    uint16_t u16_rate_mHz;
    uint16_t u16_amount;
};

#endif // LFO_H
