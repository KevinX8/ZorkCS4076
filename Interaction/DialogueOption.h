#ifndef DIALOGUEOPTION_H_
#define DIALOGUEOPTION_H_

#include <vector>

using namespace std;

template <class T> class DialogueOption
{
    private:
    public:
    const T option;
    const T reply;
    const vector<DialogueOption<T>> nextOptions;
    DialogueOption<T>(T o, T r, vector<DialogueOption<T>> nO): option(o), reply(r), nextOptions(nO){
    }
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
