#include "Update.hpp"
#include "World.hpp"

Update::Update() {
    this->func_ = [this] { this->update(); };
    this->get_world().updates.request_set(&this->func_);
}
Update::~Update() {
    this->get_world().updates.request_erase(&this->func_);
}
Update::Update(const Update & /*other*/) {
    this->func_ = [this] { this->update(); };
    this->get_world().updates.request_set(&this->func_);
}
