// Base class for all Monocle C++ classes.

#ifndef CLASS_Object
#define CLASS_Object

class Object
{
        private:
                long m_ReferenceCount;

        public:
                inline Object()
                {
                        this->m_ReferenceCount = 0;
                }

                inline void Grab()
                {
                        this->m_ReferenceCount += 1;
                }

                inline void Drop()
                {
                        this->m_ReferenceCount -= 1;

                        if (this->m_ReferenceCount <= 0)
                                delete this;
                }
};

#endif