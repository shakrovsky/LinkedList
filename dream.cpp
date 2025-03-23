#include <iostream>

// Containers - пространство имен, предназначенное для хранения различных контейнеров.
namespace Containers
{
    // LinkedList - шаблонный класс, описывающий структуру однонаправленного связанного списка.
    template <typename T>
    class LinkedList
    {
    private:
        // ListNode - cтруктура узла.
        template <typename Type>
        struct ListNode
        {
            // Значение, хранящееся в узле.
            Type value;

            // Указатель на следующий узел списка.
            ListNode<Type>* next;

            ListNode(const Type& value) : value(value), next(nullptr) {}
        };

        // Размер списка на текущий момент.
        size_t sizeOfList;

        // Указатель на первый узел списка.
        ListNode<T>* head;

        // Указатель на последний узел списка.
        ListNode<T>* tail;

    public:
        /*  Iterator - класс, описывающий структуру итератора 
            (объекта, с помощью которого можно итерироваться по списку).  */
        class Iterator
        {
        private:
            // Указатель на узел, на который смотрит итератор.
            ListNode<T>* pointerToNode;

        public:
            // Информация об итераторе для библиотеки <algorithm>:
            using iterator_category = std::forward_iterator_tag;   // Тип итератора.
            using value_type = T;                                  // Тип элемента.
            using difference_type = std::ptrdiff_t;                // Разница между итераторами.
            using pointer = T*;                                    // Указатель на элемент.
            using reference = T&;                                  // Ссылка на элемент.

            Iterator(ListNode<T>* somePointer) : pointerToNode(somePointer) {}

            // Оператор разыменования - возвращает значение узла (на который смотрит итератор) по ссылке.
            reference operator*() {
                return pointerToNode->value;
            }

            // Оператор префиксного инкремента - передвигает итератор на следующий узел списка.
            Iterator& operator++()
            {
                pointerToNode = pointerToNode->next;
                return *this;
            }

            // Операторы сравнения - проверяют итераторы на равенство.
            bool operator==(const Iterator& other) {
                return pointerToNode == other.pointerToNode;
            }

            bool operator!=(const Iterator& other) {
                return pointerToNode != other.pointerToNode;    
            }
        };

        // Метод возвращает итератор, который смотрит на голову списка.
        Iterator begin() const {
            return Iterator(head);
        }

        /*  Метод возвращает итератор, который смотрит на узел, 
            следующий за хвостом (это всегда nullptr).  */
        Iterator end() const {
            return Iterator(nullptr);
        }

        // Конструктор по умолчанию.
        LinkedList() : sizeOfList(0), head(nullptr), tail(nullptr) {}

        // Пользовательский конструктор.
        LinkedList(const std::initializer_list<T>& list) : LinkedList()
        {
            for (const T& value: list) {
                this->pushBack(value);
            }
        }

        // Конструктор глубокого копирования.
        LinkedList(const LinkedList<T>& other) 
        : sizeOfList(other.sizeOfList), head(nullptr), tail(nullptr)
        {
            // 1. Если другой список пустой - инициализации выше достаточно.
            if (other.isEmpty()) {
                return;
            }

            // В противном случае:

            /* 2.   Создаю новый узел со значением головы другого списка. 
                    Голову и хвост текущего списка направляю на созданный узел.   */
            head = new ListNode<T>(other.head->value);
            tail = head;

            // 3. Создаю временный указатель на узлы другого списка (для итерации по ним).
            ListNode<T>* currentOther = other.head->next;

            /* 4.   Итерируюсь по узлам другого списка, беру из них значения, 
                    и с помощью метода pushBack() добавляю новые узлы c взятыми значениями в свой список.  */
            while (currentOther != nullptr)
            {
                this->pushBack(currentOther->value);
                currentOther = currentOther->next;
            }
        }

        // Конструктор копирования перемещением.
        LinkedList(LinkedList<T>&& other) noexcept
        : sizeOfList(other.sizeOfList), head(other.head), tail(other.tail)
        {
            // 1. С помощью списка инициализации я забираю ресурсы у объекта other.

            /* 2.   Для объекта other я обнуляю размер списка, указатели на голову и хвост.
                    Благодаря данным манипуляциям, деструктор объекта other не сможет освободить 
                    занимаемые им ресурсы (см. реализацию деструктора). 

                    Это полностью корректно, так как данными ресурсами теперь владеет наш список.  */
            other.sizeOfList = 0;
            other.head = nullptr;
            other.tail = nullptr;
        }

        // Деструктор.
        ~LinkedList() {
            this->clear();
        }

        // Оператор глубокого копирования.
        LinkedList<T>& operator=(const LinkedList<T>& other)
        {
            // 1. Если произошла попытка самоприсваивания - ничего не делаю.
            if (this == &other) {
                return *this;
            }

            // В противном случае:

            // 2. Очищаю свой список с помощью метода clear() (который грамотно удаляет все узлы).
            this->clear();

            /* 3.   Создаю новый узел со значением головы другого списка. 
                    Голову и хвост текущего списка направляю на созданный узел.   */
            head = new ListNode<T>(other.head->value);
            tail = head;

            // 4. Создаю временный указатель на узлы другого списка (для итерации по ним).
            ListNode<T>* currentOther = other.head->next;

            /* 5.   Итерируюсь по узлам другого списка, беру из них значения, 
                    и с помощью метода pushBack() добавляю новые узлы c взятыми значениями в свой список.   */
            while (currentOther != nullptr)
            {
                this->pushBack(currentOther->value);
                currentOther = currentOther->next;
            }

            return *this;
        }

        // Оператор присваивания перемещением.
        LinkedList<T>& operator=(LinkedList<T>&& other) noexcept
        {
            // 1. Если произошла попытка самоприсваивания - ничего не делаю.
            if (this == &other) {
                return *this;
            }

            // В противном случае:

            // 2. Очищаю свой список с помощью метода clear() (который грамотно удаляет все узлы).
            this->clear();

            // 3. Забираю ресурсы, которыми владеет объект other. 
            sizeOfList = other.sizeOfList;
            head = other.head;
            tail = other.tail;

            /* 4.   Для объекта other я обнуляю размер списка, указатели на голову и хвост.
                    Благодаря данным манипуляциям, деструктор объекта other не сможет освободить 
                    занимаемые им ресурсы (см. реализацию деструктора). 

                    Это полностью корректно, так как данными ресурсами теперь владеет наш список.  */
            other.sizeOfList = 0;
            other.head = nullptr;
            other.tail = nullptr;

            return *this;
        }

        /*
        1.  Данный оператор предоставляет доступ к элементам списка по индексу (аналогично массивам).
        2.  Доступна как положительная индексация (первый элемент имеет индекс 0),
        так и отрицательная (последний элемент имеет индекс -1).
        3.  Оператор возвращает элемент по ссылке.
        4.  Важно! Обращение к элементу по индексу имеет сложность O(n), что может быть неэффективно для больших списков.
        */
        T& operator[](int index)
        {
            // 1. Если значение индекса выходит за пределы - выбрасываю исключение.
            if (index < -int(sizeOfList) || index >= int(sizeOfList)) {
                throw std::out_of_range("Error! The index is out of range.");
            }

            // 2. Создаю временный указатель, с помощью которого буду итерироваться по списку.
            ListNode<T>* tempPtr = head;

            // 3. Итерируюсь по списку, нахожу нужный мне узел и возвращаю его значение по ссылке.
            for (int i = 0; i < sizeOfList * (index < 0) + index; ++i) {
                tempPtr = tempPtr->next;
            }

            return tempPtr->value;
        }

        /*  Метод показывает, является ли список пустым.
            Возвращает соответствующее булевое значение.  */
        bool isEmpty() const {
            return head == nullptr;
        }

        // Метод возвращает длину списка на текущий момент.
        size_t size() const {
            return sizeOfList;
        }

        // Метод возвращает значение первого узла списка по ссылке.
        T& front() const
        {
            // Если список пустой - невозможно обратиться к первому узлу (выбрасываю исключение).
            if (head == nullptr) {
                throw std::runtime_error("Error! The list is empty - you cannot refer to the first element.");
            }

            return head->value;
        }

        // Метод возвращает значение последнего узла списка по ссылке.
        T& back() const
        {
            // Если список пустой - невозможно обратиться к последнему узлу (выбрасываю исключение).
            if (head == nullptr) {
                throw std::runtime_error("Error! The list is empty - you cannot refer to the last element.");
            }

            return tail->value;
        }

        // Метод выводит значения всех узлов в порядке их расположения в списке.
        void print() const
        {
            ListNode<T>* tempPtr = head;

            while (tempPtr != nullptr)
            {
                std::cout << tempPtr->value << ' ';
                tempPtr = tempPtr->next;
            }

            std::cout << '\n';
        }

        /*  Метод ищет первый узел со значением value и возвращает указатель 
            на значение этого узла. Если узел не найден - возвращает nullptr.  */
        T* find(const T& value) const
        {
            ListNode<T>* tempPtr = head;

            while (tempPtr != nullptr)
            {
                if (tempPtr->value == value) {
                    return &tempPtr->value;
                }

                tempPtr = tempPtr->next;
            }

            return nullptr;
        }

        /*  Метод проверяет, есть ли в списке узел со значением value.
            Возвращает соответствующее булевое значение.  */
        bool contains(const T& value) const {
            return find(value) != nullptr;
        }

        /*  Метод сортирует список, используя пузырьковую сортировку.
            Важно! Данный алгоритм имеет сложность O(n^2), что может быть неэффективно для больших списков.  */
        void sort()
        {
            // 1. Если в списке меньше 2-х элементов - сортировать ничего не нужно.
            if (sizeOfList < 2) {
                return;
            }

            // Длина списка как минимум 1.
            bool isSwapped = false;

            // 2. Сортирую список.
            do
            {
                isSwapped = false;
                ListNode<T>* current = head;

                while (current->next)
                {
                    if (current->value > current->next->value)
                    {
                        std::swap(current->value, current->next->value);
                        isSwapped = true;
                    }

                    current = current->next;
                }

            } while (isSwapped);
        }

        // Метод полностью очищает список.
        void clear()
        {
            /*  - Пока список не пустой -> циклично удаляю первый узел.
                - Стоит отметить, что я использую именно метод popFront(), т.к. он удаляет 
                узел со сложностью O(1). Если бы я использовал метод popBack(), сложность удаления 
                одного узла возрасла бы до O(n) - что неээфективно.  */ 
            while (!isEmpty()) {
                popFront();
            }
        }

        /*  Метод удаляет первый узел со значением valueToRemove.
            Возвращает true, если узел с соответствующим значением был найден и удалён, иначе - false.  */
        bool remove(const T& valueToRemove)
        {
            // 1. Если список пустой - невозможно ничего удалить (возвращаю false).
            if (head == nullptr) {
                return false;
            }

            // 2. Если нужно удалить первый элемент:
            if (head->value == valueToRemove)
            {
                popFront();
                return true;
            }

            // В противном случае:

            // 3. Создаю временный указатель для итерации по списку (сначала tempPtr указывает на первый узел).
            ListNode<T>* tempPtr = head;

            /* 4. Данный цикл закончится в двух случаях:
                1) Указатель tempPtr указывает на последний узел списка.
                2) Следующий узел после tempPtr содержит значение, которое необходимо удалить. */
            while (tempPtr->next != nullptr && tempPtr->next->value != valueToRemove) {
                tempPtr = tempPtr->next;
            }

            /* ОБЪЯСНЕНИЕ ПУНКТА 4.

            ПЕРВЫЙ СЛУЧАЙ:
            - Цикл заканчивается, когда tempPtr->next == nullptr.
            - Это означает, что tempPtr указывает на последний узел списка.
            - В этом случае, значение valueToRemove не было найдено в оставшейся части списка.

            ВТОРОЙ СЛУЧАЙ:
            - Цикл заканчивается, когда tempPtr->next->value == valueToRemove.
            - Это означает, что следующий узел после tempPtr содержит значение, которое необходимо удалить.
            - В этом случае, tempPtr является предыдущим узлом по отношению к узлу, который необходимо удалить. */

            // 5. Если я вышел из цикла из-за первого случая -> значит, я ничего не нашел -> возвращаю false.
            if (tempPtr->next == nullptr) {
                return false;
            }

            // Если я зашел сюда, значит узел, следующий за tempPtr, необходимо удалить.

            // 6. Создаю указатель на узел, который необходимо удалить.
            ListNode<T>* nodeToRemove = tempPtr->next;

            // 7. Теперь текущий узел должен показывать на узел, на который показывал удаляемый узел.
            tempPtr->next = nodeToRemove->next;

            // 8. Удаляю узел со значением valueToRemove.
            delete nodeToRemove;

            // 9. Если я удалил последний узел, то необходимо обновить хвост.
            if (tempPtr->next == nullptr) {
                tail = tempPtr;
            }

            --sizeOfList;

            return true;
        }

        /*  Метод удаляет все узлы со значением valueToRemove.
            Возвращает true, если хотя бы один узел был удалён, иначе - false. */
        bool removeAll(const T& valueToRemove)
        {
            bool flag = false;

            while (contains(valueToRemove)) 
            {
                flag = true;
                remove(valueToRemove);
            }

            return flag;
        }

        // Метод добавляет новый элемент в начало списка.
        void pushFront(const T& value)
        {
            // 1. Создаю новый узел и получаю на него указатель.
            ListNode<T>* newNode = new ListNode<T>(value);

            // 2. Новый узел должен показывать на первый узел списка.
            newNode->next = head;

            // 3. Голова, в свою очередь, теперь должна показывать на новый узел.
            head = newNode;

            // 4. Если список был пустым - хвост должен показывать на новый узел.
            if (tail == nullptr) {
                tail = newNode;
            }

            ++sizeOfList;
        }

        // Метод добавляет новый элемент в конец списка.
        void pushBack(const T& value)
        {
            // 1. Создаю новый узел и получаю на него указатель.
            ListNode<T>* newNode = new ListNode<T>(value);

            // 2. Если список пустой - новый узел становится как головой, так и хвостом.
            if (head == nullptr)
            {
                head = newNode;
                tail = newNode;
            }
            else
            {
                // Если список не пустой:

                // 3. Последний узел списка должен показывать на новый узел.
                tail->next = newNode;

                /* 3.1. Указатель на хвост должен показывать на новый узел 
                        (так как теперь новый узел - последний в списке). */
                tail = newNode;
            }

            ++sizeOfList;
        }

        /*  Метод удаляет первый элемент из списка.
            Возвращает значение удаленного элемента.  */
        T popFront()
        {
            /* 1.   Проверяю список на пустоту. 
                    Если список пустой - невозможно ничего удалить (выбрасываю исключение).   */
            if (head == nullptr) {
                throw std::runtime_error("Error! It's not possible to delete item from empty list!");
            }
        
            // 2. Сохраняю значение удаляемого узла.
            T deleted = head->value;
        
            // 3. Сохраняю указатель на второй узел (так как он впоследствии станет первым).
            ListNode<T>* second = head->next;
        
            // 4. Удаляю первый узел.
            delete head;
        
            // 5. Направляю указатель на голову на второй узел.
            head = second;
        
            // 6. Если список стал пустым - обнуляю указатель на хвост.
            if (head == nullptr) {
                tail = nullptr;
            }
        
            --sizeOfList;
        
            return deleted;
        }

        /*  Метод удаляет последний элемент из списка.
            Возвращает значение удаленного элемента.  */
        T popBack()
        {
            /* 1.   Проверяю список на пустоту. 
                    Если список пустой - невозможно ничего удалить (выбрасываю исключение).   */
            if (head == nullptr) {
                throw std::runtime_error("Error! It's not possible to delete item from empty list!");
            }
        
            // 2. Переменная для хранения значения удаляемого узла.
            T deleted;
        
            // 3. Если в списке только один узел:
            if (head == tail)
            {
                // 3.1. Сохраняю значение удаляемого узла.
                deleted = head->value;
        
                // 3.2. Удаляю узел и обнуляю указатели.
                delete head;
                head = nullptr;
                tail = nullptr;
            }
            else
            {
                // 4. Если узлов больше одного:
        
                // 4.1. Ищу предпоследний узел.
                ListNode<T>* tempPtr = head;
                while (tempPtr->next != tail) {
                    tempPtr = tempPtr->next;
                }
        
                // Здесь tempPtr указывает на предпоследний узел.
        
                // 4.2. Сохраняю значение последнего узла.
                deleted = tail->value;
        
                // 4.3. Удаляю последний узел.
                delete tail;
        
                // 4.4. Обновляю указатель на хвост и обнуляю указатель хвоста.
                tail = tempPtr;
                tail->next = nullptr;
            }
        
            --sizeOfList;
        
            return deleted;
        }
    };
}
