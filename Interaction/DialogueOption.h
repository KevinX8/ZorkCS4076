#ifndef DIALOGUEOPTION_H_
#define DIALOGUEOPTION_H_

template <class T> class DiaglogueOption
{
    private:
    T option;
    T reply;
    vector<DialogueOption> nextOptions;
    public:
    template <class T> T DialogueOption::getOption() {
        return option;
    }
    template <class T> T DiaglogueOption::getReply() {
        return reply;
    }
    template <class T> vector<DiaglogueOption> DiaglogueOption::&getNextOption() {
        return nextOptions;
    }
};
#endif /*DIALOGUEOPTION_H_*/