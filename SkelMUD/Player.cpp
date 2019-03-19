#include "Player.h"
#include "Logger.h"

Player::Player() : Entity() {
    m_room_id = 1;
    m_planet_id = 0;
    m_visible = true;
}

Player::Player(int player_id, std::string player_name) {
    m_room_id = 1;
    m_planet_id = 0;
    m_player_id = player_id;
    m_name = player_name;
    m_visible = true;
}

Player::Player(int player_id, std::string player_name, std::string player_class, std::string player_race) : Entity() {
    m_player_id = player_id;
    m_name = player_name;
    m_class = player_class;
    m_race = player_race;
    m_room_id = 1;
    m_planet_id = 0;
    m_visible = true;
}

int Player::GetID() {
    return m_player_id;
}

void Player::SetID(int id) {
    m_player_id = id;
}

const std::string &Player::GetPlayerName() const {
    return m_name;
}

void Player::SetPlayerName(const std::string &name) {
    m_name = name;
}

const std::string &Player::GetPlayerRace() const {
    return m_race;
}

void Player::SetPlayerRace(const std::string &race) {
    m_race = race;
}

const std::string &Player::GetPlayerClass() const {
    return m_class;
}

void Player::SetPlayerClass(const std::string &character_class) {
    m_class = character_class;
}

bool Player::IsVisible() {
    return m_visible;
}

void Player::SetVisible(const bool visible) {
    m_visible = visible;
}

void Player::AddItem(std::string item) {
    if(m_items.find(item) == m_items.end()) {
        m_items[item] = 1;
    }
    else {
        m_items[item]++;
    }
}

void Player::AddItem(std::shared_ptr<Item> item) {
    AddItem(item->GetItemName());
}

void Player::RemoveItem(std::string item) {
    if(m_items.find(item) != m_items.end()) {
        m_items[item]--;
        if(m_items[item] == 0) {
            m_items.erase(item);
        }
    }
}

void Player::Send(std::string data) {
    m_send_queue << data;
}

std::string Player::GetCommData() {
    std::string data = m_send_queue.str();
    m_send_queue.str(std::string());
    return data;
}

std::map<std::string, int> Player::GetItems() {
    return m_items;
}

void Player::SetMainHand(std::shared_ptr<Item> item) {
    m_main_hand_item = item;
}

std::shared_ptr<Item> Player::GetMainHand() {
    return m_main_hand_item;
}

void Player::SetOffHand(std::shared_ptr<Item> item) {
    m_off_hand_item = item;
}

std::shared_ptr<Item> Player::GetOffHand() {
    return m_off_hand_item;
}
