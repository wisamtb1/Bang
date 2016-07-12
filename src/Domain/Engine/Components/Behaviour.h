#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "Bang.h"
#include "Logger.h"
#include "WindowMain.h"
#include "SingletonManager.h"
#include "ISceneEventListener.h"
#include "IWindowEventManagerListener.h"

class BehaviourHolder;

/**
 * @brief The Behaviour class is a base class which will be inherited by all
 * the User Behaviour classes. This lets the engine handle Behaviours uniformly.
 */
class Behaviour : public ISceneEventListener
                 ,public IWindowEventManagerListener  // To receive events!
{
friend class BehaviourHolder;

private:

    std::string m_sourceFilepath = "";

    /**
     * @brief Set directly by BehaviourHolder (its a friend class), when
     * adding this Behaviour to itself.
     */
    BehaviourHolder *p_behaviourHolder = nullptr;


    void Init(BehaviourHolder *bh);
    void SetSourceFilepath(const std::string &m_sourceFilepath);
    std::string GetSourceFilepath() const;

/**
 * These variables must be copied from BehaviourHolder
 */
private:
    GameObject* p_gameObject = nullptr;
    Transform*  p_transform  = nullptr;

protected:
    GameObject* const& gameObject = p_gameObject;
    Transform*  const& transform  = p_transform;
/**
*/

public:
    Behaviour();
    virtual ~Behaviour();

protected:
    virtual void OnUpdate() override;
};

#endif


// DEFINES

#define BANG_BEHAVIOUR_CLASS(CLASS_NAME) \
extern "C" Behaviour *CreateDynamically(SingletonManager *mainBinarySingletonManager) \
{ \
\
    /* This line links the SingletonManager in the main binary to the SingletonManager \
       in the behaviour loaded library. */ \
    SingletonManager::SetSingletonManagerInstanceFromBehaviourLibrary(mainBinarySingletonManager); \
\
    Behaviour *b = new CLASS_NAME(); \
    return b; \
} \
\
extern "C" void DeleteDynamically(Behaviour *b) \
{ \
    delete b; \
}












