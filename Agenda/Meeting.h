//
//  Meeting.h
//  Agenda
//
//  Created by fh on 2017/3/15.
//  Copyright © 2017年 付豪. All rights reserved.
//

#ifndef Meeting_h
#define Meeting_h

#include "Date.h"

class Meeting{
public:
    Meeting(){}
    Meeting( string sponsor,string participator,Date startDate,Date endDate,string title){
        sponsor_=sponsor;
        participator_=participator;
        startDate_=startDate;
        endDate_=endDate;
        title_=title;
    }
    string getSponsor(void) const {
        return sponsor_;
    }
    void  setSponsor(string sponsor){
        sponsor_=sponsor;
    }
    string getParticipator(void) const {
        return participator_;
    }
    void  setParticipator(string participator){
        participator_=participator;
    }
    Date getStartDate(void) const{
        return startDate_;
    }
    void setStartDate(Date startTime){
        startDate_=startTime;
    }
    Date getEndDate(void) const{
        return endDate_;
    }
    void setEndDate(Date endTime){
        endDate_=endTime;
    }
    string getTitle(void) const{
        return title_;
    }
    void serTitle(string title){
        title_=title;
    }
    
private:
    Date startDate_;
    Date endDate_;
    string sponsor_;
    string participator_;
    string title_;
};


#endif /* Meeting_h */
