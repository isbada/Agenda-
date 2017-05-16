//
//  AgendaUI.h
//  Agenda
//
//  Created by fh on 2017/3/16.
//  Copyright © 2017年 付豪. All rights reserved.
//

#ifndef AgendaUI_h
#define AgendaUI_h

#include "AgendaService.h"


class AgendaUI{
public:
    AgendaUI(){
        startAgenda();
    }
    void OperationLoop(void){
        string operation;
        do{
            cout<< "________________________________Agenda________________________________\n"
            << "Action :\n"
            << "l   - log in Agenda by user name and password\n"
            << "r   - register an Agenda account\n"
            << "q   - quit Agenda\n"
            << "______________________________________________________________________\n"
            << "\n" << "Agenda : ~$ ";
            cin>>operation;
            switch(operation[0]){
                case'l':
                    userLogIn();
                    break;
                case'r':
                    userRegister();
                    break;
                case'q':
                    quitAgenda();
                    break;
                default:
                    break;
            }
        }while(operation[0]!='q');
    }
    
private:
    AgendaService agendaService_;
    string userName_;
    string userPassword_;
    
    //-----------主界面操作-------------------------
    void startAgenda(void){
        agendaService_.startAgenda();
    }
    
    void userLogIn(void){
        string userName,password;
        cout<<"[log in] [user name] [password]\n"
        <<"[log in] ";
        cin>>userName>>password;
        if(agendaService_.userLogIn(userName,password)){
            userName_=userName;
            userPassword_=password;
            cout<<"[log in] succeed!\n";
            //这个还没解决
            while(executeOperation(getOperation()));
        }
        else{
            cout<<"[error] log in faild!\n";
        }
    }
    void userRegister(void){
        string userName,password,email,phone;
        cout<<"[register] [user name] [password] [email] [phone]\n"
        <<"[register] ";
        cin>>userName>>password>>email>>phone;
        if(agendaService_.userRegister(userName,password,email,phone)){
            userName_=userName;
            userPassword_=password;
            cout<<"[register] succeed!\n";
            while(executeOperation(getOperation()));
        }
        else{
            cout<<"[error] register faild!\n";
        }
    }
    
    void quitAgenda(void){
        agendaService_.quitAgenda();
    }
    
    //---------二级界面操作函数--------------------------
    string getOperation(){
        string operation;
        cout<<"________________________________Agenda________________________________\n"
        <<"Action :\n"
        <<"o 	- 	log out Agenda\n"
        <<"dc 	- 	delete Agenda account\n"
        <<"lu 	- 	list all Agenda user\n"
        << "cm  - create a meeting\n"
        << "la  - list all meetings\n"
        << "las - list all sponsor meetings\n"
        << "lap - list all participate meetings\n"
        << "qm  - query meeting by title\n"
        << "qt  - query meeting by time interval\n"
        << "dm  - delete meeting by title\n"
        << "da  - delete all meetings\n"
        << "______________________________________________________________________\n"
        << "\n" << "Agenda@" << userName_ << ":# ";
        cin>>operation;
        return operation;
    }
    
    bool executeOperation(string op){
        if(op=="o"){
            userLogOut();
            return false;
        }
        else if(op=="dc"){
            deleteUser();
            return false;
        }
        else if(op=="lu"){
            listAllUsers();
            return true;
        }
        else if(op=="cm"){
            createMeetings();
            return true;
        }
        else if(op=="la"){
            listAllMeetings();
            return true;
        }
        else if(op=="las"){
            listAllSponsorMeetings();
            return true;
        }
        else if(op=="lap"){
            listAllParticipateMeetings();
            return true;
        }
        else if(op=="qm"){
            queryMeetingByTitle();
            return true;
        }
        else if(op=="qt"){
            queryMeetingByTimeInterval();
            return true;
        }
        else if(op=="dm"){
            deleteMeetingByTitle();
            return true;
        }
        else if(op=="da"){
            deleteAllMeetings();
            return true;
        }
        else
            cout<<"Please Select Correct-Operation!\n";
        
        return true;
    }
    
    void userLogOut(void){
        userName_.clear();
        userPassword_.clear();
    }
    void deleteUser(void){
        agendaService_.deleteUser(userName_,userPassword_);
        userLogOut();
    }
    void listAllUsers(void){
        printUsers(agendaService_.listAllUsers());
    }
    
    void printUsers(list<User>users){
        cout<<"name\t"<<"email\t"<<"phone\n";
        for(auto itr=users.begin();itr!=users.end();itr++){
            cout<<itr->getName()<<"\t"
            <<itr->getEmail()<<"\t"
            <<itr->getPhone()<<"\n";
        }
    }
    
    void createMeetings(void){
        string title,participator,startDate,endDate;
        cout << "[create meeting] [title] [participator] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]\n"
        << "[create meeting] ";
        cin>>title>>participator>>startDate>>endDate;
        if(agendaService_.createMeeting(userName_,title,participator,startDate,endDate))
            cout<<"[create meeting] succeed!\n";
        else{
            cout<<"[error] create meeting faild!\n";
        }
    }
    void listAllMeetings(void){
        printMeetings(agendaService_.listAllMeetings(userName_));
    }
    
    void listAllSponsorMeetings(){
        printMeetings(agendaService_.listAllSponsorMeetings(userName_));
    }
    void listAllParticipateMeetings(){
        printMeetings(agendaService_.listAllParticipateMeetings(userName_));
    }
    
    void printMeetings(list<Meeting>meetings){
        cout<<"title\t"<<"sponsor\t"<<"participator\t"<<"start time\t"<<"end time\n";
        for(auto itr=meetings.begin();itr!=meetings.end();itr++){
            cout<<itr->getTitle()<<"\t"
            <<itr->getSponsor()<<"\t"
            <<itr->getParticipator()<<"\t"
            <<Date::dateToString(itr->getStartDate())<<"\t"
            <<Date::dateToString(itr->getEndDate())<<"\n";
        }
    }
    
    void queryMeetingByTitle(void){
        string title;
        cout<<"[query meeting] [title]\n"
        <<"[query meeting] ";
        cin>>title;
        printMeetings(agendaService_.meetingQuery(userName_,title));
    }
    
    void queryMeetingByTimeInterval(void){
        string startDate,endDate;
        cout<<"[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]\n";
        cin>>startDate>>endDate;
        printMeetings(agendaService_.meetingQuery(userName_,startDate,endDate));
    }
    void deleteMeetingByTitle(void){
        string title;
        cout << "[delete meeting] [title]\n"
        << "[delete meeting] ";
        cin>>title;
        if(agendaService_.deleteMeeting(userName_,title)){
            cout<<"[delete meeting by title] succeed!\n";
        }
        else{
            cout<<"[error] delete meeting failed!\n";
        }
    }
    void deleteAllMeetings(void){
        agendaService_.deleteAllMeetings(userName_);
        cout<<"[delete all meetings] succeed!\n";
    }
    
};

#endif /* AgendaUI_h */
