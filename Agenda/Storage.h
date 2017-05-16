//
//  Storage.h
//  Agenda
//
//  Created by fh on 2017/3/16.
//  Copyright © 2017年 付豪. All rights reserved.
//

#ifndef Storage_h
#define Storage_h


#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName&); \
void operator=(const TypeName&)

#include "User.h"
#include "Meeting.h"


class Storage{
    
private:
    static Storage *instance_;
    DISALLOW_COPY_AND_ASSIGN(Storage);
    Storage(){readFromeFile("agenda.data");}
    
    list<User> userList_;
    list<Meeting> meetingList_;
    
    bool readFromeFile(const char* fpath){
        ifstream fin(fpath);
        if(!fin.is_open())
            return false;
        int userCount, meetingCount;
        string str;
        stringstream ss;
        fin>>str;
        //为什么是total + 6？？？total后面的值是usercount
        str=str.substr(str.find("total:") +6 );
        ss<<str;
        ss>>userCount;
        ss.str("");//这句清空ss
        
        for(int i=0;i<userCount;i++){
            fin>>str;
            userList_.push_back(JSONToUser(str));
        }
        
        fin>>str;
        str=str.substr(str.find("total:") +6 );
        ss<<str;
        ss>>meetingCount;
        ss.str("");//这句清空ss
        
        for(int i=0;i<meetingCount;i++){
            fin>>str;
            meetingList_.push_back(JSONToMeeting(str));
        }
        cout<<"------------------读取成功！！！！-----------"<<endl;
        cout<<fpath<<endl;
        
        return true;
    }
    
    bool writeToFile(const char *fpath){
        ofstream fout(fpath);
        if(!fout.is_open())	return false;
        fout<<"{collection:\"User\",total:"<<userList_.size()<<"}\n";
        //写user数据
        for(auto itr=userList_.begin();itr!=userList_.end();itr++){
            fout<<UserToJSON(*itr)<<endl;
        }
        //写meeting数据
        fout << "{colletion:\"Meeting\",total:" << meetingList_.size() << "}\n";
        for (auto itr = meetingList_.begin(); itr != meetingList_.end(); itr++) {
            fout << MeetingToJSON(*itr) << endl;
        }
        
        cout<<"------------------写入成功！！！！-----------"<<endl;
        cout<<fpath<<endl;
        return true;
    }
    
    string UserToJSON(const User&user){
        stringstream ss;
        ss<<"{name:"<<"\""<<user.getName()<<"\","
        <<"password:"<<"\""<<user.getPassWord()<<"\","
        <<"email:"<<"\""<<user.getEmail()<<"\","
        <<"phone:"<<"\""<<user.getPhone()<<"\"}";
        return ss.str();
    }
    string MeetingToJSON(const Meeting& meeting){
        stringstream ss;
        ss<<"{sponsor:"<<"\""<<meeting.getSponsor()<<"\","
        <<"partipator:"<<"\""<<meeting.getParticipator()<<"\","
        <<"sdate:"<<"\""<<Date::dateToString(meeting.getStartDate())<<"\","
        <<"edate:"<<"\""<<Date::dateToString(meeting.getEndDate())<<"\","
        <<"title:"<<"\""<<meeting.getTitle()<<"\"}";
        return ss.str();
    }
    User JSONToUser(const string&json){
        string name,password,email,phone;
        int pos,len;
        
        pos=json.find("name:\"")+6;
        len=json.find("\",password")-pos;
        name=json.substr(pos,len);
        
        pos = json.find("password:\"") + 10;
        len = json.find("\",email") - pos;
        password = json.substr(pos, len);
        
        pos = json.find("email:\"") + 7;
        len = json.find("\",phone") - pos;
        email = json.substr(pos, len);
        
        pos = json.find("phone:\"") + 7;
        len = json.find("\"}") - pos;
        phone = json.substr(pos, len);
        
        return User(name,password,email,phone);
    }
    Meeting JSONToMeeting(const string&json){
        string sponsor, partipator, sdate, edate, title;
        int pos, len;
        pos = json.find("sponsor:\"") + 9;
        len = json.find("\",partipator") - pos;
        sponsor = json.substr(pos, len);
        pos = json.find("partipator:\"") + 12;
        len = json.find("\",sdate") - pos;
        partipator = json.substr(pos, len);
        pos = json.find("sdate:\"") + 7;
        len = json.find("\",edate") - pos;
        sdate = json.substr(pos, len);
        pos = json.find("edate:\"") + 7;
        len = json.find("\",title") - pos;
        edate = json.substr(pos, len);
        pos = json.find("title:\"") + 7;
        len = json.find("\"}") - pos;
        title = json.substr(pos, len);
        return Meeting(sponsor, partipator,
                       Date::stringToDate(sdate), Date::stringToDate(edate), title);
    }
    
public:
    static Storage *getInstance(void){
        if(instance_==nullptr)//c++里面空指针的标准表达nullptr
            instance_=new Storage();
        return instance_;
    }
    ~Storage(){}
    //CQUD四个操作
    void createUser(const User&user){
        userList_.push_back(user);
    }
    
    list<User> queryUser(function<bool(const User&)>filter){
        list<User>returnList;
        
        for(auto itr=userList_.begin();itr!= userList_.end(); ++itr){
            if(filter(*itr))	returnList.push_back(*itr);
        }
        return returnList;
    }
    //update返回的是更新的用户数量
    int updateUser(function<bool(const User&)>filter,function<void(User&)>switcher){
        int count=0;
        for (auto itr = userList_.begin(); itr != userList_.end(); ++itr)
        {//filter?switcher
            if(filter(*itr)){
                switcher(*itr);	count++;
            }
        }
        return count;
    }
    int deleteUser(function<bool(const User&)>filter){
        int count=0;
        for (auto itr = userList_.begin(); itr != userList_.end(); ++itr){
            if(filter(*itr)){
                userList_.erase(itr);//erase
                count++;
            }
        }
        return count;
    }
    //--------meeting 的CQUD
    void createMeeting(const Meeting&meeting){
        meetingList_.push_back(meeting);
    }
    list<Meeting> queryMeeting(
                               std::function<bool(const Meeting&)> filter) {
        list<Meeting> returnList;
        for (auto itr = meetingList_.begin(); itr != meetingList_.end(); itr++) {
            if (filter(*itr)) {
                returnList.push_back(*itr);
            }
        }
        return returnList;
    }
    
    int updateMeeting(function<bool(const Meeting&)>filter,function<void(const Meeting&)>switcher){
        int count=0;
        for (auto itr = meetingList_.begin(); itr != meetingList_.end(); itr++) {
            if (filter(*itr)) {
                switcher(*itr);
                count++;
            }
        }
        return count;
    } 
    
    int deleteMeeting(function<bool(const Meeting&)> filter) {
        int count = 0;
        for (auto itr = meetingList_.begin(); itr != meetingList_.end(); itr++) {
            if (filter(*itr)) {
                meetingList_.erase(itr);
                count++;
            }
        }
        return count;
    }
    
    bool sync(void){
        return writeToFile("agenda.data");
    }
};

Storage* Storage::instance_=nullptr;






#endif /* Storage_h */
