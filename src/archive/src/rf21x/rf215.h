#ifndef __RF215_H__
#define __RF215_H__

#include "rf21x-base.h"
#include "gsp.h"

class RF215 : public RF21x, public GspRecvEvent
{
public:
    explicit RF215();
    virtual ~RF215();

public:
    virtual bool open(const std::string port, int minId, int maxId);
    virtual bool close();

    virtual bool startQuiz(RF21xQuizType type, const RF21xQuizParams &params);
    virtual bool startQuizDirectly(RF21xQuizType type, void *buffer);
    virtual bool stopQuiz();

    virtual bool setKeypadId(int id);

	virtual unsigned char GetQuizResult(int id,int QuizNo);
	virtual void ClearQuizResult();

protected:
    Gsp _sp;
    bool _sendCommand(unsigned char *cmd, int cmdLength,
                      int waitTime = 100,
                      unsigned char *ret = NULL, int retLength = 0);

protected:
    virtual void dataRecvEvent(AbstractGsp *gsp, unsigned char *data, int length);
    bool _checkPackage(unsigned char *data, int retLength);
    void _processPackage(unsigned char *data, int length);

protected:
    RF21xQuizType _type;
};

#endif
