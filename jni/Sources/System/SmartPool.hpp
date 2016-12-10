#ifndef KE_SMARTPOOL_HPP
#define KE_SMARTPOOL_HPP

#include <functional>
#include <memory>
#include <vector>

namespace ke
{

template <typename T>
class SmartPool
{
    public:
        /// \brief Constructor
        /// \param filter Filter to determine which objects can be handled by the pool
        /// \param parent SmartPool parent of this pool
        SmartPool(std::function<bool(std::shared_ptr<T> obj)> filter = [](std::shared_ptr<T> obj)->bool { return true; }, SmartPool<T>* parent = nullptr)
        : mFilter(filter)
        , mParent(parent)
        , mChildren()
        , mObjects()
        {
            if (mParent != nullptr)
            {
                mParent->addChild(this);
            }
        }

        /// \brief Constructor
        /// \param parent SmartPool parent of this pool
        /// \param filter Filter to determine which objects can be handled by the pool
        SmartPool(SmartPool<T>& parent, std::function<bool(std::shared_ptr<T> obj)> filter = [](std::shared_ptr<T> obj)->bool { return true; })
        : mFilter(filter)
        , mParent(&parent)
        , mChildren()
        , mObjects()
        {
            parent.addChild(this);
        }

        /// \brief Default Destructor
        ~SmartPool()
        {
            // Remove all the objects (recursively)
            clear();

            // Remove all the children (recursively)
            std::size_t children = mChildren.size();
            for (std::size_t i = 0; i < children; i++)
            {
                mChildren.erase(mChildren.begin());
            }

            // Remove this if it is a child of some pool
            if (mParent != nullptr)
            {
                mParent->removeChild(this);
            }
        }

        /// \brief Add an object
        /// \param obj The object
        void add(std::shared_ptr<T> obj)
        {
            if (!contains(obj) && mFilter)
            {
                if (mFilter(obj))
                {
                    mObjects.push_back(obj);
                    for (std::size_t i = 0; i < mChildren.size(); i++)
                    {
                        mChildren[i]->add(obj);
                    }
                }
            }
        }

        /// \brief Remove an object
        /// \param obj The object
        void remove(std::shared_ptr<T> obj)
        {
            for (std::size_t i = 0; i < mChildren.size(); i++)
            {
                mChildren[i]->remove(obj);
            }
            for (std::size_t i = 0; i < mObjects.size();)
            {
                if (mObjects[i] == obj)
                {
                    mObjects.erase(mObjects.begin() + i);
                }
                else
                {
                    i++;
                }
            }
        }


        /// \brief Remove all the objects
        void clear()
        {
            for (std::size_t i = 0; i < mChildren.size(); i++)
            {
                mChildren[i]->clear();
            }

            mObjects.clear();
        }

        /// \brief Does the pool contain the object ?
        /// \param obj The object
        /// \return True if contained, else false
        bool contains(std::shared_ptr<T> obj) const
        {
            for (std::size_t i = 0; i < mObjects.size(); i++)
            {
                if (mObjects.at(i) == obj)
                {
                    return true;
                }
            }
            return false;
        }

        /// \brief Get the size of the pool
        /// \return The size of the pool
        std::size_t size() const
        {
            return mObjects.size();
        }

        /// \brief Get the object with the given index
        /// \param index The index
        /// \return The object or nullptr if wrong index
        std::shared_ptr<T> at(std::size_t index) const
        {
            if (0 <= index && index < mObjects.size())
            {
                return mObjects.at(index);
            }
            return nullptr;
        }

        /// \brief Get the object with the given index
        /// \param index The index
        /// \return The object or nullptr if wrong index
        std::shared_ptr<T> operator[](std::size_t index) const
        {
            return at(index);
        }

        /// \brief Add a child
        /// \param child The child
        void addChild(SmartPool<T>* child)
        {
            if (child != nullptr)
            {
                child->mParent = this;
                for (std::size_t i = 0; i < mObjects.size(); i++)
                {
                    child->add(mObjects[i]);
                }
                mChildren.push_back(child);
            }
        }

        /// \brief Remove a child
        /// \param child The child
        void removeChild(SmartPool<T>* child)
        {
            for (std::size_t i = 0; i < mChildren.size();)
            {
                if (mChildren[i] == child)
                {
                    mChildren.erase(mChildren.begin() + i);
                }
                else
                {
                    i++;
                }
            }
        }

        /// \brief Get the number of children
        /// \return The number of children
        std::size_t children() const
        {
            return mChildren.size();
        }

    private:
        std::function<bool(std::shared_ptr<T> obj)> mFilter;
        SmartPool<T>* mParent;
        std::vector<SmartPool<T>*> mChildren;
        std::vector<std::shared_ptr<T>> mObjects;

};

} // namespace ke

#endif // KE_SMARTPOOL_HPP
