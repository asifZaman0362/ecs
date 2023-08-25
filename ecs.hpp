#ifndef ECS_HPP
#define ECS_HPP

#include <algorithm>
#include <array>
#include <bitset>
#include <cstddef>
#include <deque>
#include <list>
#include <memory>
#include <set>
#include <unordered_map>
#include <iostream>

#define MAX_ENTITIES 10000
#define MAX_COMPONENTS 32

using Entity = unsigned long;
using Signature = std::bitset<MAX_COMPONENTS>;

class IComponentArray {
   public:
    virtual ~IComponentArray() = default;
    virtual void OnEntityDelete(const Entity) = 0;
};

template <typename T>
class ComponentList : public IComponentArray {

    friend class Registry;

   public:
    void OnEntityDelete(const Entity) override;
    void AddComponent(const Entity, const T &);
    void RemoveComponent(const Entity);
    T& GetComponent(Entity e);

   private:
    std::array<T, MAX_ENTITIES> m_array;
    std::unordered_map<Entity, size_t> m_entityToIndexMap;
    std::unordered_map<size_t, Entity> m_indexToEntityMap;
    size_t m_length = 0;
};

template <typename T>
inline size_t GetId();

class ISystem {
   protected:
    std::set<Entity> m_entities;

   public:
    virtual Signature GetSignature() = 0;
    virtual void update(float) = 0;
    virtual ~ISystem() = default;
    void AddEntity(Entity e);
    void RemoveEntity(Entity e);
};

class Registry {
   public:
    Registry();

    template <typename T>
    static size_t RegisterComponent();

    template <typename T>
    static std::shared_ptr<ComponentList<T>> GetComponentArray();

    template <typename T>
    static T& GetComponent(Entity e);

    template <typename T>
    static void AddComponent(Entity e, T component);

    template <typename T>
    static void RemoveComponent(Entity e);

    static void EntityDestroyed(Entity e);

    static Entity CreateEntity();

    static std::set<Entity> filter(std::bitset<MAX_COMPONENTS>);

   private:
    static std::array<size_t, MAX_COMPONENTS> registered_components;
    static std::unordered_map<size_t, std::shared_ptr<IComponentArray>>
        component_lists;
    static std::deque<Entity> available_entities;
    static std::unordered_map<Entity, Signature> entity_signatures;
    template <typename T>
    static bool is_comp_registered();
};

class Coordinator {
   private:
    static std::unordered_map<size_t, std::shared_ptr<ISystem>> systems;
    static std::unordered_map<size_t, Signature> system_signatures;

   public:
    template <typename T>
    static bool LoadSystem(std::shared_ptr<ISystem> system);
    template <typename T>
    static bool UnloadSystem();
    static void EntityUpdated(Entity, Signature, Signature);
    static void update(float dt);
    static void OnEntityDestroyed(Entity);
};

inline size_t counter = 1;
inline size_t system_counter = 1;

template <typename T>
inline size_t GetId() {
    static size_t id = counter++;
    return id;
}

template <typename T>
inline size_t GetSystemId() {
    static size_t id = system_counter++;
    return id;
}

template <typename T>
void ComponentList<T>::OnEntityDelete(const Entity e) {
    if (m_entityToIndexMap.find(e) != m_entityToIndexMap.end())
        RemoveComponent(e);
}

inline int y(int a) {
    return a * 10;
}

template <typename T>
void ComponentList<T>::AddComponent(const Entity e, const T &comp) {
    m_array[m_length] = comp;
    m_entityToIndexMap[e] = m_length;
    m_indexToEntityMap[m_length] = e;
    m_length++;
}

template <typename T>
void ComponentList<T>::RemoveComponent(const Entity e) {
    m_array[e] = m_array[m_length - 1];
    auto lastEntity = m_indexToEntityMap[m_length - 1];
    auto indexOfLastEntity = m_entityToIndexMap[lastEntity];
    auto indexOfRemovedEntity = m_entityToIndexMap[e];
    m_entityToIndexMap[lastEntity] = indexOfRemovedEntity;
    m_indexToEntityMap[indexOfRemovedEntity] = lastEntity;
    m_entityToIndexMap.erase(e);
    m_indexToEntityMap.erase(indexOfLastEntity);
    m_length--;
}

template <typename T>
T& ComponentList<T>::GetComponent(Entity e) {
    if (m_entityToIndexMap.find(e) != m_entityToIndexMap.end()) {
        return m_array[m_entityToIndexMap[e]];
    }
}

template <typename T>
size_t Registry::RegisterComponent() {
    auto id = GetId<T>();
    if (!is_comp_registered<T>()) {
        component_lists[id] = std::make_shared<ComponentList<T>>();
    }
    return id;
}

template <typename T>
std::shared_ptr<ComponentList<T>> Registry::GetComponentArray() {
    auto id = GetId<T>();
    if (is_comp_registered<T>()) {
        std::cout << component_lists[id];
        if (component_lists[id] == nullptr) {
            return nullptr;
        }
        return std::static_pointer_cast<ComponentList<T>>(component_lists[id]);
    } else
        return nullptr;
}

template <typename T>
T& Registry::GetComponent(Entity e) {
    return GetComponentArray<T>()->GetComponent(e);
}

template <typename T>
void Registry::AddComponent(Entity e, T component) {
    auto id = GetId<T>();
    if (is_comp_registered<T>()) {
        if (entity_signatures.find(e) == entity_signatures.end()) {
            Signature signature;
            signature.set(id, true);
            entity_signatures[e] = signature;
            Coordinator::EntityUpdated(e, signature, 0);
        } else {
            auto signature = entity_signatures[e];
            auto old = signature;
            signature.set(id, true);
            Coordinator::EntityUpdated(e, signature, old);
        }
        auto array = GetComponentArray<T>();
        if (array)
            GetComponentArray<T>()->AddComponent(e, component);
        else {
            std::cout << "fuck";
        }
    }
}

template <typename T>
void Registry::RemoveComponent(Entity e) {
    auto id = GetId<T>();
    if (is_comp_registered<T>()) {
        auto signature = entity_signatures[e];
        auto old = signature;
        signature.set(GetId<T>(), false);
        Coordinator::EntityUpdated(e, signature, old);
        GetComponentArray<T>()->RemoveComponent(e);
    }
}

template <typename T>
bool Registry::is_comp_registered() {
    auto id = GetId<T>();
    //return std::find(registered_components.begin(), registered_components.end(),
    //                 id) != std::end(registered_components);
    return component_lists.find(id) != component_lists.end();
}

template <typename T>
bool Coordinator::LoadSystem(std::shared_ptr<ISystem> system) {
    auto id = GetSystemId<T>();
    if (systems.find(id) == std::end(systems)) {
        system_signatures[id] = system->GetSignature();
        systems[id] = system;
        return true;
    }
    return false;
}

template <typename T>
bool Coordinator::UnloadSystem() {
    auto id = GetSystemId<T>();
    if (systems.find(id) != std::end(systems)) {
        systems.erase(id);
        return true;
    }
    return false;
}

#endif
