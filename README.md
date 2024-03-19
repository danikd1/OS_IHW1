# ИДЗ 1. Каналы. Очереди сообщений.

**Дельцов Даниил Витальевич БПИ226**
**Вариант 29**

Условие: азработать программу, которая определяет частоту встречаемости (сколько раз встретилось в тексте) пяти ключевых слов языка программирования C, в произвольной ASCII–строке.
Ключевые слова не должны являться частью идентификаторов. Пять искомых ключевых слов выбрать по своему усмотрению. Тестировать можно на файлах программ.
В выходном тексте для каждого ключевого слова вывести, сколько раз оно встретилось в обработанном тексте.

**4 балла** - ihw1
Программа состоит из трех основных процессов: чтения, обработки и записи данных. Процесс чтения данных связан с процессом их обработки через канал fdReader,
а процесс обработки данных, в свою очередь, связан с процессом записи через канал fdWriter. Взаимодействие процессов чтения и записи с соответствующими файлами осуществляется с помощью функций read_file и write_file.

- [x] первый процесс читает текстовые данные из заданного файла и через неименованный канал передает их второму процессу;
- [x] второй процесс осуществляет обработку данных в соответствии с вариантом задания и передает результат обработки через неименованный канал третьему процессу;
- [x] третий процесс осуществляет вывод данных в заданный файл.

Чтобы запустить программу из командной строки, требуется указать имя файла для чтения и имя файла для записи результата. Если аргументы переданы неверно, программа уведомит пользователя об ошибке. 
В процессе тестирования программы использовался ряд текстовых файлов. Для каждого тестового файла входных данных существует соответствующий выходной файл, который был сгенерирован в ходе работы программы.

**5 баллаов** - ihw1_v2
Программа состоит из трех основных процессов: чтения, обработки и записи данных.
Между процессом чтения и обработки установлена связь через именованный канал first.fifo, а связь между процессом обработки и записи осуществляется через именованный канал second.fifo. 
Для создания этих каналов используется команда mknod. Взаимодействие с файлами для чтения и записи данных происходит благодаря функциям read_file и write_file.

- [x] первый процесс читает текстовые данные из заданного файла и через именованный канал передает их второму процессу;
- [x] второй процесс осуществляет обработку данных в соответствии с заданием и передает результат обработки через именован- ный канал третьему процессу;
- [x] третий процесс осуществляет вывод данных в заданный файл.

**6 баллаов** - ihw1_v3
В программе задействованы два процесса: обработка файлов и обработка строк. Они соединены между собой двумя каналами: fdReaderHandler используется для передачи строки, считанной из файла,
а fdHandlerWriter применяется для отправки обработанной строки в процесс, отвечающий за запись в файл.
Функции read_file и write_file отвечают за работу процесса обработки файлов с файловой системой,выполняя чтение из файлов и запись в файлы соответственно.

- [x] первый процесс читает текстовые данные из заданного файла и через неименованный канал передает их второму процессу;
- [x] второй процесс осуществляет обработку данных в соответствии с заданием и передает результат обработки через неимено- ванный канал обратно первому процессу;
- [x] первый процесс осуществляет вывод данных в заданный файл.

**7 баллаов** - ihw1_v4
В этой программе задействованы два основных процесса: один занимается обработкой файлов, а другой — обработкой текстовых строк. 
Для взаимодействия между этими процессами используются именованные каналы: first.fifo служит для передачи строк, извлеченных из файла, в то время как через second.fifo 
передаются строки после их обработки для дальнейшей записи в файл. Создание этих каналов осуществляется с использованием функции mknod. Взаимосвязь процесса, ответственного за файлы, 
с самими файлами устанавливается через функции read_file для чтения и write_file для записи.

- [x] первый процесс читает текстовые данные из заданного файла и через именованный канал передает их второму процессу;
- [x] второй процесс осуществляет обработку данных в соответствии с заданием и передает результат обработки через именованный канал обратно первому процессу;
- [x] первый процесс осуществляет вывод данных в заданный файл.

**8 баллаов** - ihw1_v5
Программа состоит из двух отдельных процессов: один отвечает за работу с файлами, другой — за обработку текстовых данных. 
Для обмена информацией между этими процессами используются именованные каналы: first.fifo передает строку, считанную из файла, в процесс обработки строки,
а second.fifo используется для отправки обработанной строки обратно в процесс, который занимается записью данных в файл.
Каналы создаются при помощи функции mknod. Взаимодействие процесса, занимающегося файлами, с этими файлами осуществляется через функции read_file для чтения и write_file для записи.
Код, реализующий процесс обработки файлов, находится в файле TextHancler, в то время как логика обработки строк размещена в файле main.c.

- [x] первый процесс читает текстовые данные из заданного файла и через именованный канал передает их второму процессу;
- [x] второй процесс осуществляет обработку данных в соответствии с заданием и передает результат обработки через именован- ный канал обратно первому процессу;
- [x] первый процесс осуществляет вывод данных в заданный файл.

