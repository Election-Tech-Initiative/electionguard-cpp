#include "electionguard/encrypt.hpp"
#include <iostream>

electionguard::EncryptionCompositor::EncryptionCompositor() {
    std::cout<<__func__<<" : Creating EncryptionCompositor["<<this<<"]"<<std::endl;
}

electionguard::EncryptionCompositor::~EncryptionCompositor() {
    std::cout<<__func__<<" : Destroying EncryptionCompositor["<<this<<"]"<<std::endl;
}

int electionguard::EncryptionCompositor::encrypt() {
    std::cout<<__func__<<" : encrypting by instance "<<std::endl;
    return 9;
}

int electionguard::encrypt_ballot() {
    std::cout<<__func__<<" : encrypting by function "<<std::endl;
    return 8;
}
