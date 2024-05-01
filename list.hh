#ifndef LIST
#define LIST

// Obsolete, use std::vector!
// Has memory leaks!
template <typename T>
class List{
    private:
        T* ptr;
        unsigned int capacity = 1;
        static List<void*> ptrs;
    public:
        static int destruct;
        unsigned int count = 0;
        List(){
            capacity = 1;
            count = 0;
            ptr = new T[capacity];
        }
        List(int initCapacity){
            capacity = initCapacity;
            count = 0;
            ptr = new T[capacity];
        }
        ~List(){
            count = 0;
            capacity = 0;
            ptrs.Add((void*)ptr);
            if(destruct)
                delete[] ptr;
        }
        void Add(T element){
            if(count +1 > capacity){
                capacity *= 2;
                T* tmp = new T[capacity];
                for(unsigned int i =0; i < count;i++)
                    tmp[i] = ptr[i];
                delete[] ptr;
                ptr = tmp;
            }
            ptr[count] = element;
            count++;
        }
        void Remove(unsigned int element){
            for(unsigned int i = element; i < count - 2; i++)
                ptr[i] = ptr[i+1];
            count -= 1;
        }
        void Clean(){
            if(count < (capacity / 2)){
                capacity /= 2;
                T* tmp = new T[capacity];
                for(unsigned int i =0; i < count;i++)
                    tmp[i] = ptr[i];
                delete[] ptr;
                ptr = tmp;
            }
        }
        void Clear(){
            delete[] ptr;
            capacity = 1;
            ptr = new T[capacity];
        }
        void Clear(unsigned int initCap){
            delete[] ptr;
            capacity = initCap;
            ptr = new T[capacity];
        }
        static void CleanUp(){
            for(unsigned int i =0; i < ptrs.count; i++)
                delete[] (T*)ptrs[i];
            ptrs.Clear();
        }
        T& operator[](unsigned int index){return ptr[index];}
        T* GetPtr(){return ptr;}
};
template<typename T>
int List<T>::destruct;
template<typename T>
List<void*> List<T>::ptrs;
#endif
