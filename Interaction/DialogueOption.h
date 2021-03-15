#ifndef DIALOGUEOPTION_H_
#define DIALOGUEOPTION_H_

#include <vector>

template <class T> class DialogueOption
{
    private:
    T option;
    T reply;
    vector<DialogueOption<T>> nextOptions;
    public:
    T getOption() {
        return option;
    }
    T getReply() {
        return reply;
    }
    vector<DialogueOption<T>> getNextOption() {
        return nextOptions;
    }
};
#endif /*DIALOGUEOPTION_H_*/