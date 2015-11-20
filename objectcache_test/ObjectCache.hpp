#include <vector>
#include <map>
#include <set>
#include <stdint.h>
#include "sigslot.hpp"
#include "EventableObject.hpp"

namespace base {

#define DEFAULT_TIMEOUT (2)
#define DEFAULT_SURVIVAL (3600)
#define DEFAULT_WATER_MARK (7*1024)

template <typename KeyType, typename InfoType>
class ObjectCache;

/*CacheCollector 处理cache收集的基类*/
template<typename KeyType, typename InfoType>
class CacheCollector: public sigslot::has_slots<> {
public:
    typedef std::set<KeyType> RequestSet;

    typedef std::map<KeyType, InfoType> InfoMap;

    typedef std::map<KeyType, InfoType> ResultType;

    typedef ObjectCache<KeyType, InfoType> CacheManagerType;

    CacheCollector(const RequestSet& requests):
        m_requests(requests) {
    };

    bool operator<(const CacheCollector<KeyType, InfoType>& other) {
        return this < &other;
    };

    bool operator==(const CacheCollector<KeyType, InfoType>& other) {
        return this == &other;
    };

    bool collected() {
        return m_requests.size() == m_infos.size() + m_errors.size();
    };

    void on_load_info(const KeyType& key, const InfoType& info) {
        m_infos[key] = info;
        m_errors.erase(key);
        if(collected()) {
            callback();
        }
    }

    void on_error(const KeyType& key) {
        m_errors.insert(key);
        if(collected()) {
            callback();
        }
    }

    virtual void callback()=0;

protected:
    InfoMap m_infos;

    RequestSet m_requests;

    RequestSet m_errors;
};

template<typename CallBackClass, typename KeyType,typename InfoType>
class CacheCollector_P0: public CacheCollector<KeyType, InfoType> {
public:
    typedef void (CallBackClass::*HanlderType)(const typename CacheCollector<KeyType, InfoType>::ResultType&, const typename CacheCollector<KeyType, InfoType>::RequestSet& errors);

    CacheCollector_P0(const typename CacheCollector<KeyType, InfoType>::RequestSet& requests,
                        typename CacheCollector<KeyType, InfoType>::CacheManagerType* pcache,
                        CallBackClass* pclass, HanlderType handler);

    void on_obj_del(EventableObject* obj);

    sigslot::signal1< CacheCollector<KeyType, InfoType>* > sig_colletor_del;

    void callback();

private:
    CallBackClass *m_class;

    HanlderType m_handler;

    typename CacheCollector<KeyType, InfoType>::CacheManagerType* m_cache;
};

template<typename CallBackClass, typename KeyType,typename InfoType>
CacheCollector_P0<CallBackClass, KeyType, InfoType>::CacheCollector_P0(const typename CacheCollector<KeyType, InfoType>::RequestSet& requests,
                        typename CacheCollector<KeyType, InfoType>::CacheManagerType* pcache,
                        CallBackClass* pclass, HanlderType handler) :
    CacheCollector<KeyType, InfoType>(requests),
    m_class(pclass),
    m_handler(handler),
    m_cache(pcache) {
    pclass->sig_event_disable.connect(this, &CacheCollector_P0<CallBackClass, KeyType, InfoType>::on_obj_del);
    sig_colletor_del.connect(pcache, &CacheCollector<KeyType, InfoType>::CacheManagerType::release );
}

template<typename CallBackClass, typename KeyType,typename InfoType>
void CacheCollector_P0<CallBackClass, KeyType, InfoType>::on_obj_del(EventableObject* obj) {
    sig_colletor_del(this);
    delete this;
}
    
template<typename CallBackClass, typename KeyType,typename InfoType>
void CacheCollector_P0<CallBackClass, KeyType, InfoType>::callback() {
    (m_class->*m_handler)(CacheCollector<KeyType, InfoType>::m_infos, CacheCollector<KeyType, InfoType>::m_errors);
    sig_colletor_del(this);
    delete this;
}

template<typename CallBackClass, typename KeyType,typename InfoType, typename P1>
class CacheCollector_P1: public CacheCollector<KeyType, InfoType> {
public:
    typedef void (CallBackClass::*HanlderType)(const typename CacheCollector<KeyType, InfoType>::ResultType&, const typename CacheCollector<KeyType, InfoType>::RequestSet& errors, P1);

    CacheCollector_P1(const typename CacheCollector<KeyType, InfoType>::RequestSet& requests,
                        typename CacheCollector<KeyType, InfoType>::CacheManagerType* pcache,
                        CallBackClass* pclass, HanlderType handler, const P1& p1);

    void on_obj_del(EventableObject* obj);

    sigslot::signal1<CacheCollector<KeyType, InfoType>*> sig_colletor_del;

    void callback();
private:
    CallBackClass *m_class;

    HanlderType m_handler;

    P1 m_p1;
    
    typename CacheCollector<KeyType, InfoType>::CacheManagerType* m_cache;
};

template<typename CallBackClass, typename KeyType, typename InfoType, typename P1>
CacheCollector_P1<CallBackClass, KeyType, InfoType, P1>::CacheCollector_P1(const typename CacheCollector<KeyType, InfoType>::RequestSet& requests,
                        typename CacheCollector<KeyType, InfoType>::CacheManagerType* pcache,
                        CallBackClass* pclass, HanlderType handler, const P1& p1) :
    CacheCollector<KeyType, InfoType>(requests),
    m_class(pclass),
    m_handler(handler),
    m_p1(p1),
    m_cache(pcache) {
    pclass->sig_event_disable.connect(this, &CacheCollector_P1<CallBackClass, KeyType, InfoType, P1>::on_obj_del);
    sig_colletor_del.connect(pcache, &CacheCollector<KeyType, InfoType>::CacheManagerType::release );
}

template<typename CallBackClass, typename KeyType, typename InfoType, typename P1>
void CacheCollector_P1<CallBackClass, KeyType, InfoType, P1>::on_obj_del(EventableObject* obj) {
    sig_colletor_del(this);
    delete this;
}
    
template<typename CallBackClass, typename KeyType,typename InfoType, typename P1>
void CacheCollector_P1<CallBackClass, KeyType, InfoType, P1>::callback() {
    (m_class->*m_handler)(CacheCollector<KeyType, InfoType>::m_infos, CacheCollector<KeyType, InfoType>::m_errors, m_p1);
    sig_colletor_del(this);
    delete this;
}

template<typename InfoType> 
class CacheInfo {
public:
    CacheInfo(const InfoType& info, uint32_t dead_time):
        m_info(info),
        dead_time(dead_time) {

    };
    InfoType m_info;
    uint32_t dead_time;
};

/*CacheTrigger cache触发器，当加载cache项的时候发送消息给CacheConnector*/
template<typename KeyType, typename InfoType>
class CacheTrigger {
public:
    uint32_t get_survival() const;

    KeyType get_key() const ;

    CacheTrigger(const KeyType& key, uint32_t survival, uint32_t timeout);

    sigslot::signal2<const KeyType&, const InfoType&> signal_trigger;
    
    sigslot::signal1<const KeyType&> signal_error;

    bool operator<(const CacheTrigger<KeyType, InfoType>& other) const;

    bool operator==(const CacheTrigger<KeyType, InfoType>& other) const;

    uint32_t m_info_survival;

    uint32_t m_timeout;
    
    uint32_t m_query_time;

private:
    KeyType m_key;
};

template<typename KeyType, typename InfoType>    
CacheTrigger<KeyType, InfoType>::CacheTrigger(const KeyType& key, uint32_t survival, uint32_t timeout):
    m_info_survival(survival),
    m_timeout(timeout),
    m_query_time(0),
    m_key(key)
{

}

template<typename KeyType, typename InfoType>    
uint32_t CacheTrigger<KeyType, InfoType>::get_survival() const {
    return m_info_survival;
}
    
template<typename KeyType, typename InfoType>    
KeyType CacheTrigger<KeyType, InfoType>::get_key() const {
    return m_key;
}

template<typename KeyType, typename InfoType>
bool CacheTrigger<KeyType, InfoType>::operator<(const CacheTrigger<KeyType, InfoType>& other) const {
    return m_key < other.m_key;
}

template<typename KeyType, typename InfoType>
bool CacheTrigger<KeyType, InfoType>::operator==(const CacheTrigger<KeyType, InfoType>& other) const {
    return m_key == other.m_key;
}

template <typename KeyType, typename InfoType>
class ObjectCache: public sigslot::has_slots<> {
public:
    typedef typename CacheCollector<KeyType, InfoType>::RequestSet RequestSet;
    typedef typename CacheCollector<KeyType, InfoType>::ResultType ResultType;

    ObjectCache(uint32_t default_timeout = DEFAULT_TIMEOUT, uint32_t default_survial = DEFAULT_SURVIVAL, uint32_t water_mark= DEFAULT_WATER_MARK);

    ~ObjectCache();

    void update(const KeyType& key, const InfoType& info, uint32_t now);

    InfoType* get(const KeyType& key);

    void config_once(const KeyType& key, uint32_t survival = DEFAULT_SURVIVAL, uint32_t timeout = DEFAULT_TIMEOUT);
    
    void reset(KeyType& key);

    void set_survival(const KeyType& key, uint32_t survival);

    template<typename CallBackClass>
    void add_callback(const RequestSet&, CallBackClass* pclass, void (CallBackClass::*method)(const ResultType&, const RequestSet&) );

    template<typename CallBackClass, typename P1>
    void add_callback(const RequestSet&, CallBackClass* pclass, void (CallBackClass::*method)(const ResultType&, const RequestSet&, P1), const P1& );

    void release(CacheCollector<KeyType, InfoType>* colletor);

    void refresh(uint32_t now_time);

    uint64_t get_visit_num();

    uint64_t get_hit_num();

    virtual    void handle_req(const RequestSet& request){};

private:
typedef std::map< CacheTrigger<KeyType, InfoType>, CacheInfo<InfoType>* > InfoMap;
    InfoMap m_info_map;

typedef std::set<CacheCollector<KeyType, InfoType>* > ColletorSet;
    ColletorSet m_colletor_set;

    uint32_t m_default_timeout;

    uint32_t m_default_survival;

    uint32_t m_water_mark;
//statistics
private:
    uint64_t m_total_visit;
    uint64_t m_total_hit;
};

template <typename KeyType, typename InfoType>
ObjectCache<KeyType, InfoType>::ObjectCache(uint32_t default_timeout, uint32_t default_survial, uint32_t water_mark):
    m_default_timeout(default_timeout),
    m_default_survival(default_survial),
    m_water_mark(water_mark) {
}
    
template <typename KeyType, typename InfoType>
ObjectCache<KeyType, InfoType>::~ObjectCache() {
    typename ColletorSet::iterator itr = m_colletor_set.begin();
    for(; itr!=m_colletor_set.end(); ++itr) {
        delete (*itr);
    }
}

template <typename KeyType, typename InfoType>
void ObjectCache<KeyType, InfoType>::update(const KeyType& key, const InfoType& info, uint32_t now) {
    CacheTrigger<KeyType, InfoType> trigger(key, m_default_survival, m_default_timeout);
    typename InfoMap::iterator itr = m_info_map.find(trigger); 
    if( itr!= m_info_map.end()) {
        if(itr->second) {
            delete itr->second;
        }
        itr->second = new CacheInfo<InfoType>(info, itr->first.get_survival()+ now);    
        (const_cast< typename InfoMap::key_type& >(itr->first)).signal_trigger(key, info);
    } else {
        m_info_map.insert(typename InfoMap::value_type(trigger, new CacheInfo<InfoType>(info, m_default_survival+ now)));
    }

};

template <typename KeyType, typename InfoType>
InfoType* ObjectCache<KeyType, InfoType>::get(const KeyType& key) {
    CacheTrigger<KeyType, InfoType> trigger(key, m_default_survival, m_default_timeout);
    typename InfoMap::iterator itr = m_info_map.find(trigger); 
    if(itr!=m_info_map.end()) {
        if(itr->second) {
            return &(itr->second->m_info);
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}

template <typename KeyType, typename InfoType>
void ObjectCache<KeyType, InfoType>::reset(KeyType& key) {
    CacheTrigger<KeyType, InfoType> trigger(key, m_default_survival, m_default_timeout);
    typename InfoMap::iterator itr = m_info_map.find(trigger);
    if(itr!=m_info_map.end()) {
        if(itr->second) {
            delete itr->second;
            itr->second = NULL;
        }
    }
}

template <typename KeyType, typename InfoType>
void ObjectCache<KeyType, InfoType>::config_once(const KeyType& key, uint32_t survival, uint32_t timeout) {
    CacheTrigger<KeyType, InfoType> trigger(key, survival, timeout);
    typename InfoMap::iterator itr = m_info_map.insert(trigger).second;
    if(itr!= m_info_map.end()) {
        itr->first.m_info_survival = survival;
        itr->first.m_timeout = timeout;
    }
}
    
template <typename KeyType, typename InfoType>
template<typename CallBackClass>
void ObjectCache<KeyType, InfoType>::add_callback(const RequestSet& requests, CallBackClass* pclass, void (CallBackClass::*method)(const ResultType&, const RequestSet&) ) {
    CacheCollector_P0<CallBackClass, KeyType, InfoType>* pcolletor = new CacheCollector_P0<CallBackClass, KeyType, InfoType>(requests, this, pclass, method);
    bool finish = true;
    typename RequestSet::const_iterator itr = requests.begin();
    for (; itr!=requests.end(); ++itr) {
        m_total_visit ++;
        CacheTrigger<KeyType, InfoType> trigger(*itr, m_default_survival, m_default_timeout);
        typename InfoMap::iterator info_itr = m_info_map.insert(typename InfoMap::value_type(trigger, (CacheInfo<InfoType> *)NULL)).first;
        if(info_itr != m_info_map.end()) {
            (const_cast< typename InfoMap::key_type& >(info_itr->first)).signal_trigger.connect((CacheCollector<KeyType, InfoType>*)pcolletor,&CacheCollector<KeyType, InfoType>::on_load_info);
            (const_cast< typename InfoMap::key_type& >(info_itr->first)).signal_error.connect((CacheCollector<KeyType, InfoType>*)(pcolletor),&CacheCollector<KeyType, InfoType>::on_error);
            if(info_itr->second) {
                pcolletor->on_load_info(*itr,info_itr->second->m_info);
                m_total_hit ++;
            } else {
                finish = false;
            }
        }
    }
    if( !finish ) {
        m_colletor_set.insert(pcolletor);
    }
}

template <typename KeyType, typename InfoType>
template<typename CallBackClass, typename P1>
void ObjectCache<KeyType, InfoType>::add_callback(const RequestSet& requests, CallBackClass* pclass, void (CallBackClass::*method)(const ResultType&, const RequestSet&, P1), const P1& p1 ) {
    CacheCollector_P1<CallBackClass, KeyType, InfoType, P1>* pcolletor = new CacheCollector_P1<CallBackClass, KeyType, InfoType, P1>(requests, this, pclass, method, p1);
    typename RequestSet::const_iterator itr = requests.begin();
    bool finish = true;
    for (; itr!=requests.end(); ++itr) {
        m_total_visit ++;
        CacheTrigger<KeyType, InfoType> trigger(*itr, m_default_survival, m_default_timeout);
        typename InfoMap::iterator info_itr = m_info_map.insert(typename InfoMap::value_type(trigger, (CacheInfo<InfoType> *)NULL)).first;
        if(info_itr != m_info_map.end()) {
            (const_cast< typename InfoMap::key_type& >(info_itr->first)).signal_trigger.connect((CacheCollector<KeyType, InfoType>*)pcolletor,&CacheCollector<KeyType, InfoType>::on_load_info);
            (const_cast< typename InfoMap::key_type& >(info_itr->first)).signal_error.connect((CacheCollector<KeyType, InfoType>*)pcolletor,&CacheCollector<KeyType, InfoType>::on_error);
            if(info_itr->second) {
                pcolletor->on_load_info(*itr,info_itr->second->m_info);
                m_total_hit ++;
            } else {
                finish = false;
            }
        } 
    }
    if( !finish ) {
        m_colletor_set.insert(pcolletor);
    }
}
    
template <typename KeyType, typename InfoType>
void ObjectCache<KeyType, InfoType>::release(CacheCollector<KeyType, InfoType>* colletor) {
    m_colletor_set.erase(colletor);
}

template <typename KeyType, typename InfoType>
uint64_t ObjectCache<KeyType, InfoType>::get_visit_num() {
    return m_total_visit;
}

template <typename KeyType, typename InfoType>
uint64_t ObjectCache<KeyType, InfoType>::get_hit_num() {
    return m_total_hit;
}

template <typename KeyType, typename InfoType>
void ObjectCache<KeyType, InfoType>::refresh(uint32_t now_time) {
    static uint32_t last_refresh_time = 0;
    if(now_time == last_refresh_time){
        return;
    } else {
        last_refresh_time = now_time;    
    }
    RequestSet request_set;
    size_t count=0;
    typename InfoMap::iterator itr = m_info_map.begin();
    for(; itr!=m_info_map.end(); ) {
        if(itr->second && itr->second->dead_time <= now_time) {
            delete itr->second;
            if((const_cast< typename InfoMap::key_type& >(itr->first)).signal_trigger.is_empty()) {
                m_info_map.erase(itr++);
                continue;
            }else {
                itr->second = NULL;
            }
        } else if ( !itr->second && !(const_cast< typename InfoMap::key_type& >(itr->first)).signal_trigger.is_empty() && !itr->first.m_query_time) {
            (const_cast< typename InfoMap::key_type& >(itr->first)).m_query_time = now_time;
            request_set.insert(itr->first.get_key());    
        } else if ( !itr->second && !(const_cast< typename InfoMap::key_type& >(itr->first)).signal_error.is_empty() 
                && itr->first.m_timeout + itr->first.m_query_time <= now_time ) {
            (const_cast< typename InfoMap::key_type& >(itr->first)).signal_error(itr->first.get_key());
            m_info_map.erase(itr++);
            continue;
        } else {
            count++;
        }
        itr ++;
    }
    handle_req(request_set);
}

}