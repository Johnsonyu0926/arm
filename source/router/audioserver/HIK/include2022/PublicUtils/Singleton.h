/**  @file   Singleton.h
 *   @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved. 
 *   @brief  单例模板，全局类基类
 *   
 *   @author shijianfengyf2
 *   @modify shijianfengyf2
 *   @date   2018/08/09
 *   
 *   @note:
 */

#include "HPR/HPR_Config.h"
#include "HPR/HPR_Types.h"

namespace DevComUtils
{
    template <typename T>
    class CSingleton
    {
    public:
        static T* GetInstance();
        virtual ~CSingleton();
    protected:
        CSingleton(){}; //阻止别人进行构造函数
    };

    template <typename T>
    T* CSingleton<T>::GetInstance()
    {
        static T instance;  //这里不考虑线程安全问题，不上锁
                            //因为在EHome组件里，单例都是全局的globalctrl类
                            //而这个类在init的时候就会调用，此时一般是只有一个线程
        return &instance;
    }

}