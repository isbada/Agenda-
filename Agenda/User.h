//
//  User.h
//  Agenda
//
//  Created by fh on 2017/3/15.
//  Copyright © 2017年 付豪. All rights reserved.
//

#ifndef User_h
#define User_h


using namespace std;

class User {
public:
    User() {}
    User(string userName, string userPassword,
         string userEmail, string userPhone) {
        name_ = userName;
        password_ = userPassword;
        email_ = userEmail;
        phone_ = userPhone;
    }
    string getName() const {
        return name_;
    }
    void setName(string name) {
        name_ = name;
    }
    string getPassWord() const {
        return password_;
    }
    void setPassword(string password) {
        password_ = password;
    }
    string getEmail() const {
        return email_;
    }
    void setEmail(string email) {
        email_ = email;
    }
    string getPhone() const {
        return phone_;
    }
    void setPhone(string phone) {
        phone_ = phone;
    }
private:
    string name_;
    string password_;
    string email_;
    string phone_;
};



#endif /* User_h */
