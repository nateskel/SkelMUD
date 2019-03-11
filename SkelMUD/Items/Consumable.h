//
// Created by drax on 2/28/19.
//

#ifndef SKELMUD_CONSUMABLE_H
#define SKELMUD_CONSUMABLE_H


class Consumable {
public:
    Consumable();

    int GetM_hp() const;

    void SetM_hp(int m_hp);

    int GetM_max_hp() const;

    void SetM_max_hp(int m_max_hp);

    int GetM_sp() const;

    void SetM_sp(int m_sp);

    int GetM_max_sp() const;

    void SetM_max_sp(int m_max_sp);

    int GetM_stam() const;

    void SetM_stam(int m_stam);

    int GetM_max_stam() const;

    void SetM_max_stam(int m_max_stam);

    int GetM_str() const;

    void SetM_str(int m_str);

    int GetM_end() const;

    void SetM_end(int m_end);

    int GetM_int() const;

    void SetM_int(int m_int);

    int GetM_dex() const;

    void SetM_dex(int m_dex);

    int GetM_skill() const;

    void SetM_skill(int m_skill);

    int GetM_timer() const;

    void SetM_timer(int m_timer);

private:
    int m_hp;
    int m_max_hp;
    int m_sp;
    int m_max_sp;
    int m_stam;
    int m_max_stam;
    int m_str;
    int m_end;
    int m_int;
    int m_dex;
    int m_skill;
    int m_timer;
};


#endif //SKELMUD_CONSUMABLE_H
