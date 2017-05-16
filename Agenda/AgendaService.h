//
//  AgendaService.h
//  Agenda
//
//  Created by fh on 2017/3/16.
//  Copyright © 2017年 付豪. All rights reserved.
//

#ifndef AgendaService_h
#define AgendaService_h

#include "Storage.h"


using namespace std;

class AgendaService{
public:
    AgendaService(){
        startAgenda();
    }
    ~AgendaService(){ }
    void startAgenda(void){
        storage_=Storage::getInstance();
    }
    void quitAgenda(void){
        storage_->sync();
    }
    
    //------------User的DLL--------------------------------
    bool userLogIn(string userName,string password){
        //注意函数模板的使用啊！！！！
        //用的时候function<bool(const User&)>filter
        auto filter=[userName,password](const User&user){
            return user.getName()==userName&&user.getPassWord()==password;
        };
        return storage_->queryUser(filter).size()>0;
    }
    
    bool userRegister(string userName,string passWord,string email, string phone){
        auto filter=[userName,passWord,email,phone](const User&user){
            
            return user.getName()==userName||user.getPassWord()==passWord||user.getEmail()==email||user.getPhone()==phone;
        };
        if(storage_->queryUser(filter).size()==0){
            storage_->createUser(User(userName,passWord,email,phone));
            return true;
        }
        return false;
    }
    //用户只能删除自己的账号 删除账号时所有的会议自然也清空了deleteAllMeet
    bool deleteUser(string userName,string passWord){
        auto filter=[userName,passWord](const User& user){
            return user.getName()==userName&&user.getPassWord()==passWord;
        };
        if(storage_->deleteUser(filter)>0){
            deleteAllMeetings(userName);//数据库？
            return true;
        }
        return false;
    }
    
    list<User> listAllUsers(void){
        auto filter=[](const User& user){
            return true;
        };
        return storage_->queryUser(filter);
    }
    
    //----------meeting 的DLL--------------------
    bool createMeeting(string userName,string title,string participator,string startDate,string endDate){
        
        if(!Date::isValid(Date::stringToDate(startDate))||
           !Date::isValid(Date::stringToDate(endDate))||
           Date::stringToDate(startDate)>=Date::stringToDate(endDate))
            return false;
        //不能邀请并不存在的用户参与会议
        auto filter1=[participator](const User&user){
            return user.getName()==participator;
        };
        if (storage_->queryUser(filter1).size()<=0){
            return false;
        }
        //查看会议之前有没有被创建,会议title是唯一的标识
        auto filter2=[title](const Meeting&meeting){
            return meeting.getTitle()==title;
        };
        if(storage_->queryMeeting(filter2).size()>0)
            return false;
        //不能创建时间段有重复的会议（不管是发起者和参与者时间都不能重复）
        if(meetingQuery(userName,startDate,endDate).size()>0||
           meetingQuery(participator,startDate,endDate).size()>0)
            return false;
        //所有条件满足 创建会议
        storage_->createMeeting(Meeting(userName,participator,Date::stringToDate(startDate),Date::stringToDate(endDate),title));
        return true;
    }
    //createMeeting的附属函数line82 83，这个重载函数用于UI层根据标题查询会议
    list<Meeting> meetingQuery(string userName,string title){
        auto filter=[userName,title](const Meeting&meeting){
            return (meeting.getSponsor()==userName||
                    meeting.getParticipator()==userName)&&
            meeting.getTitle()==title;
        };
        return storage_->queryMeeting(filter);
    }
    //重载!用作创建会议时候判断合法性
    list<Meeting> meetingQuery(string userName,string startDate,string endDate){
        auto filter=[userName,startDate,endDate](const Meeting&meeting){
            return (meeting.getSponsor()==userName||meeting.getParticipator()==userName)&&
            ((meeting.getStartDate()<Date::stringToDate(startDate)&&meeting.getEndDate()>Date::stringToDate(startDate))||(meeting.getStartDate()<Date::stringToDate(endDate)&&meeting.getEndDate()>Date::stringToDate(endDate))
             );
        };
        return storage_->queryMeeting(filter);
    }
    
    list<Meeting> listAllMeetings(string userName){
        auto filter=[userName](const Meeting&meeting){
            return meeting.getSponsor()==userName||
            meeting.getParticipator()==userName;
        };
        return storage_->queryMeeting(filter);
    }
    list<Meeting> listAllSponsorMeetings(string userName){
        auto filter=[userName] (const Meeting&meeting){
            return meeting.getSponsor()==userName;
        };
        return storage_->queryMeeting(filter);
    }
    list<Meeting> listAllParticipateMeetings(string userName){
        auto filter=[userName](const Meeting&meeting){
            return meeting.getParticipator()==userName;
        };
        return storage_->queryMeeting(filter);
    }
    
    bool deleteMeeting(string userName,string title){
        auto filter=[userName,title](const Meeting&meeting){
            return meeting.getSponsor()==userName&&meeting.getTitle()==title;
        };//只能删除自己创建的会议，被邀请的会议自己是没办法删除的
        return storage_->deleteMeeting(filter)>0;
    }
    
    bool deleteAllMeetings(string userName){
        auto filter=[userName](const Meeting&meeting){
            return meeting.getSponsor()==userName||
            meeting.getParticipator()==userName;
        };
        return storage_->deleteMeeting(filter)>0;
    }
    
private:
    Storage *storage_;
};



#endif /* AgendaService_h */
