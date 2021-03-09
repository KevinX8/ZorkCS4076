#ifndef DIALOGUEOPTION_H_
#define DIALOGUEOPTION_H_

template <class T> class DiaglogueOption
{
    private:
    T option;
    T reply;
    vector<dialogueOption> nextOptions;
    public:
    template <class T> T DialogueOption::getOption() {
        return option;
    }
    template <class T> T DiaglogueOption::getReply() {
        return reply;
    }
    template <class T> T DiaglogueOption::&getNextOption() {
        return nextOptions;
    }
};
#endif /*DIALOGUEOPTION_H_*/