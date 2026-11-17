# Лабораторная работа №2: Модели. CRUD-операции и ORM Django

**Дисциплина:** Разработка web-приложений  
**Студент:** Гаджиев Магомед Исламович  
**Группа:** ПИЖ-б-о-25-2(2)  
**Вариант:** 6  
**Технология:** Python + Django + SQLite  

---

## Содержание
1. [Цель работы](#цель-работы)
2. [Теоретическое обоснование](#теоретическое-обоснование)
3. [Структура проекта](#структура-проекта)
4. [Пошаговое выполнение работы](#пошаговое-выполнение-работы)
5. [Выполнение индивидуального задания (Вариант 6)](#выполнение-индивидуального-задания-вариант-6)
6. [Тестирование и скриншоты работы](#тестирование-и-скриншоты-работы)
7. [Контрольные вопросы](#контрольные-вопросы)
8. [Вывод](#вывод)
9. [Список использованных источников](#список-использованных-источников)

---

## Цель работы
Разработать модели данных в Django, освоить механизм создания и применения миграций базы данных, настроить обработку медиафайлов и изучить выполнение базовых операций CRUD (Create, Read, Update, Delete) через ORM в интерактивной консоли Django (`shell`) с контролем состояния БД в SQLiteStudio.

---

## Теоретическое обоснование
В архитектуре **MTV (Model-Template-View)** модель (**Model**) отвечает за структуру данных и бизнес-логику приложения. Каждая модель представляет собой Python-класс, наследуемый от `django.db.models.Model`, и отображается в отдельную таблицу реляционной СУБД, а атрибуты класса (`CharField`, `TextField`, `DateTimeField`, `ImageField`, `BooleanField`) определяют столбцы таблицы.

Синхронизация структуры Python-моделей со схемой базы данных выполняется через систему **миграций** (`makemigrations` формирует сценарий изменений, а `migrate` применяет его к БД). Взаимодействие с записями осуществляется через **Django ORM (Object-Relational Mapping)** и менеджер объектов `objects`, который позволяет выполнять ленивые (lazy) запросы без написания сырого SQL-кода: создавать записи (`create()`, `save()`), читать и фильтровать выборки `QuerySet` (`all()`, `filter()`, `get()`, `exclude()`), обновлять атрибуты и удалять объекты (`delete()`).

---

## Структура проекта
```text
MyProject/
├── screenshots/                            # Скриншоты (в хронологическом порядке)
│   ├── 01_sqlitestudio_empty_db.png        # 1. Подключение пустой БД в SQLiteStudio
│   ├── 02_model_news_and_pillow.png        # 2. Код модели News и установка Pillow
│   ├── 03_makemigrations_sqlmigrate.png    # 3. Генерация 0001_initial.py и sqlmigrate
│   ├── 04_migrate_console.png              # 4. Применение миграций (migrate)
│   ├── 05_sqlitestudio_table_news.png      # 5. Таблица news_news и её DDL в SQLiteStudio
│   ├── 06_media_settings_urls.png          # 6. Настройка MEDIA_ROOT, MEDIA_URL и static()
│   ├── 07_shell_create_queries.png         # 7. Создание записей 1-5 в shell и connection.queries
│   ├── 08_sqlitestudio_created_rows.png    # 8. Записи 1-5 в таблице SQLiteStudio
│   ├── 09_shell_read_all_str.png           # 9. Метод __str__, вызов all() и цикл for
│   ├── 10_shell_filter_get.png             # 10. Добавление Новости 6, методы filter() и get()
│   ├── 11_update_record_7.png              # 11. Обновление записи id=7 в shell и в БД
│   ├── 12_delete_and_exclude.png           # 12. Удаление записи id=7 и метод exclude()
│   └── 13_variant6_review_crud.png         # 13. Индивидуальное задание (Вариант 6 — Review)
├── venv/                                   # Изолированное виртуальное окружение
└── MySite/                                 # Корневой каталог проекта (Sources Root)
    ├── manage.py                           # Утилита управления проектом
    ├── db.sqlite3                          # Файл базы данных SQLite 3
    ├── media/                              # Каталог загружаемых медиафайлов (photos/%Y/%m/%d/)
    ├── MySite/                             # Пакет конфигурации проекта
    │   ├── settings.py                     # Настройки БД, INSTALLED_APPS, MEDIA_ROOT/URL
    │   └── urls.py                         # Главный маршрутизатор + раздача медиа в DEBUG
    └── news/                               # Приложение новостей
        ├── migrations/
        │   └── 0001_initial.py             # Сгенерированный файл миграции модели
        ├── models.py                       # Описание моделей News и Review (Вариант 6)
        ├── urls.py                         # Локальные маршруты приложения
        └── views.py                        # Контроллеры приложения
```

---

## Пошаговое выполнение работы

### Шаг 1. Подключение базы данных в SQLiteStudio
В портативном менеджере **SQLiteStudio** подключён файл базы данных проекта `MySite/db.sqlite3`. До выполнения миграций список таблиц в базе данных пуст.

### Шаг 2. Разработка модели `News` (`news/models.py`) и установка `Pillow`
В файле `news/models.py` описан класс модели `News` с полями различных типов и магическим методом `__str__()` для строкового представления объектов:
```python
from django.db import models

class News(models.Model):
    title = models.CharField(max_length=150)
    content = models.TextField(blank=True)
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)
    photo = models.ImageField(upload_to='photos/%Y/%m/%d/')
    is_published = models.BooleanField(default=True)

    def __str__(self):
        return self.title
```
Для работы поля `ImageField` (устранения ошибки `fields.E210`) в виртуальное окружение установлена библиотека обработки изображений `Pillow`:
```cmd
pip install Pillow
```

### Шаг 3. Создание и применение миграций
Генерация файла миграции `news/migrations/0001_initial.py`, просмотр сгенерированного SQL-запроса (`CREATE TABLE "news_news" ...`) и применение миграций к базе данных `db.sqlite3`:
```cmd
python manage.py makemigrations
python manage.py sqlmigrate news 0001
python manage.py migrate
```

### Шаг 4. Настройка медиафайлов (`settings.py` и `urls.py`)
1. В конец файла `MySite/settings.py` добавлены пути хранения и раздачи загружаемых изображений:
```python
import os

MEDIA_ROOT = os.path.join(BASE_DIR, 'media')
MEDIA_URL = '/media/'
```
2. В корневой маршрутизатор `MySite/urls.py` добавлено обслуживание медиафайлов в режиме отладки (`DEBUG = True`):
```python
from django.conf import settings
from django.conf.urls.static import static
from django.contrib import admin
from django.urls import path, include

urlpatterns = [
    path('admin/', admin.site.urls),
    path('news/', include('news.urls')),
]

if settings.DEBUG:
    urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
```

### Шаг 5. Работа с ORM в интерактивной консоли (`python manage.py shell`)
Запуск интерактивной консоли выполнен командой `python manage.py shell`. Протестированы все операции **CRUD**:

```python
from news.models import News
from django.db import connection

# 1. CREATE — три способа создания записей:
# Способ 1: через конструктор с именованными аргументами + save()
news1 = News(title='Новость 1', content='Контент новости 1')
news1.save()
news2 = News(title='Новость 2', content='Контент новости 2')
news2.save()

# Способ 2: создание пустого экземпляра, заполнение атрибутов + save()
news3 = News()
news3.title = 'Новость 3'
news3.content = 'Контент новости 3'
news3.save()

# Способ 3: через менеджер объектов objects.create() (сохраняет сразу в БД)
news4 = News.objects.create(title='Новость 4', content='Контент новости 4')
News.objects.create(title='Новость 5', content='Контент новости 5')

# Просмотр выполненных SQL-запросов:
connection.queries

# 2. READ — чтение всех записей, фильтрация и получение одного объекта:
news = News.objects.all()
for item in news:
    print(item.title, item.is_published)

# Добавление записей с одинаковым заголовком для проверки фильтрации:
News.objects.create(title='Новость 6', content='Контент новости 6')
News.objects.create(title='Новость 6', content='Контент новости 7')

News.objects.filter(title='Новость 6')  # Выборка по критерию (2 записи)
News.objects.get(pk=5)                  # Получение одной записи по первичному ключу

# 3. UPDATE — обновление существующей записи (id=7):
news7 = News.objects.get(pk=7)
news7.title = 'Новость 7'
news7.save()

# 4. DELETE и EXCLUDE — удаление записи и исключение из выборки:
news7.delete()                                # Удаление записи с id=7
News.objects.exclude(title='Новость 4')       # Все записи, кроме 'Новость 4'
```

---

## Выполнение индивидуального задания (Вариант 6)

### Условие задания (Вариант 6 — Отзывы `Review`)
1. Создать модель `Review` с полями: `product_name`, `author`, `rating` (целое), `text`, `created_at`.
2. Добавить поле `useful_count` (целое число, по умолчанию `0`).
3. Выполнить миграции, наполнить таблицу через `shell` и найти все отзывы с рейтингом выше `3`.

### Код модели (`news/models.py`) и работа в `shell`
```python
class Review(models.Model):
    product_name = models.CharField(max_length=150)
    author = models.CharField(max_length=100)
    rating = models.IntegerField()
    text = models.TextField()
    created_at = models.DateTimeField(auto_now_add=True)
    useful_count = models.IntegerField(default=0)

    def __str__(self):
        return f"{self.product_name} — {self.author} ({self.rating}/5)"
```

После выполнения `python manage.py makemigrations` и `python manage.py migrate` в консоли `shell` проведены CRUD-операции и фильтрация с использованием field lookup `rating__gt=3`:
```python
from news.models import Review

Review.objects.create(product_name='Ноутбук ASUS', author='Магомед', rating=5, text='Отличный ноутбук', useful_count=4)
Review.objects.create(product_name='Мышь проводная', author='Иван', rating=2, text='Быстро сломалось колёсико')
Review.objects.create(product_name='Клавиатура Keychron', author='Алексей', rating=4, text='Удобные свитчи', useful_count=2)

# Поиск всех отзывов с рейтингом строго выше 3:
high_rated = Review.objects.filter(rating__gt=3)
print(high_rated)
# Вывод: <QuerySet [<Review: Ноутбук ASUS — Магомед (5/5)>, <Review: Клавиатура Keychron — Алексей (4/5)>]>
```

---

## Тестирование и скриншоты работы

| Этап / Операция ORM | Команда / Метод | Ожидаемый результат в БД / Консоли | Статус |
| :--- | :--- | :--- | :---: |
| Генерация и применение миграций | `makemigrations`, `migrate` | Создана таблица `news_news` (7 столбцов) | Успешно |
| Создание записей (`Create`) | `News(...)` + `save()`, `objects.create()` | В таблицу добавлены записи с `id` от `1` до `7` | Успешно |
| Чтение всех записей (`Read`) | `News.objects.all()` | Возвращён `QuerySet` со строковыми заголовками | Успешно |
| Фильтрация и выборка (`Filter` / `Get`) | `filter(title='Новость 6')`, `get(pk=5)` | Найдены 2 записи `'Новость 6'` и объект `<News: Новость 5>` | Успешно |
| Обновление записи (`Update`) | `news7.title = 'Новость 7'` + `save()` | Поле `title` и `updated_at` записи `id=7` обновлены | Успешно |
| Удаление и исключение (`Delete` / `Exclude`) | `news7.delete()`, `exclude(title='Новость 4')` | Запись `id=7` удалена; запись `id=4` исключена из выдачи | Успешно |
| Индивидуальное задание (Вариант 6) | `Review.objects.filter(rating__gt=3)` | Выведены отзывы только с рейтингом `4` и `5` | Успешно |

### Скриншоты выполнения (в хронологическом порядке)

1. **Подключение файла БД `db.sqlite3` в менеджере SQLiteStudio (пустая БД до миграций):**  
   `<img src="screenshots/01_sqlitestudio_empty_db.png" alt="Подключение пустой БД в SQLiteStudio">`  
   `![Подключение пустой БД в SQLiteStudio](screenshots/01_sqlitestudio_empty_db.png)`

2. **Описание модели `News` в `news/models.py` и установка библиотеки `Pillow`:**  
   `<img src="screenshots/02_model_news_and_pillow.png" alt="Модель News и установка Pillow">`  
   `![Модель News и установка Pillow](screenshots/02_model_news_and_pillow.png)`

3. **Создание файла миграции (`makemigrations`) и просмотр SQL-запроса (`sqlmigrate news 0001`):**  
   `<img src="screenshots/03_makemigrations_sqlmigrate.png" alt="Создание миграции и просмотр SQL">`  
   `![Создание миграции и просмотр SQL](screenshots/03_makemigrations_sqlmigrate.png)`

4. **Применение миграций к базе данных (`python manage.py migrate`):**  
   `<img src="screenshots/04_migrate_console.png" alt="Выполнение команды migrate">`  
   `![Выполнение команды migrate](screenshots/04_migrate_console.png)`

5. **Структура созданной таблицы `news_news` и её DDL-код в SQLiteStudio:**  
   `<img src="screenshots/05_sqlitestudio_table_news.png" alt="Структура и DDL таблицы news_news">`  
   `![Структура и DDL таблицы news_news](screenshots/05_sqlitestudio_table_news.png)`

6. **Настройка `MEDIA_ROOT`, `MEDIA_URL` в `settings.py` и маршрута `static()` в `urls.py`:**  
   `<img src="screenshots/06_media_settings_urls.png" alt="Настройка медиафайлов">`  
   `![Настройка медиафайлов](screenshots/06_media_settings_urls.png)`

7. **Запуск `shell`, добавление записей (`Новость 1` – `Новость 5`) и вывод `connection.queries`:**  
   `<img src="screenshots/07_shell_create_queries.png" alt="Создание записей в Django shell">`  
   `![Создание записей в Django shell](screenshots/07_shell_create_queries.png)`

8. **Проверка добавленных записей (`Новость 1` – `Новость 5`) в SQLiteStudio:**  
   `<img src="screenshots/08_sqlitestudio_created_rows.png" alt="Добавленные записи в SQLiteStudio">`  
   `![Добавленные записи в SQLiteStudio](screenshots/08_sqlitestudio_created_rows.png)`

9. **Переопределение метода `__str__()`, чтение всех записей (`all()`) и вывод в цикле `for`:**  
   `<img src="screenshots/09_shell_read_all_str.png" alt="Чтение записей и работа метода __str__">`  
   `![Чтение записей и работа метода __str__](screenshots/09_shell_read_all_str.png)`

10. **Добавление дубликатов `'Новость 6'` и работа методов `filter()` и `get(pk=5)`:**  
    `<img src="screenshots/10_shell_filter_get.png" alt="Работа методов filter и get">`  
    `![Работа методов filter и get](screenshots/10_shell_filter_get.png)`

11. **Обновление записи с `id=7` (`news7.title = 'Новость 7'` и `save()`) в `shell` и в SQLiteStudio:**  
    `<img src="screenshots/11_update_record_7.png" alt="Обновление записи id=7">`  
    `![Обновление записи id=7](screenshots/11_update_record_7.png)`

12. **Удаление записи с `id=7` (`delete()`), проверка БД и исключение записи через `exclude()`:**  
    `<img src="screenshots/12_delete_and_exclude.png" alt="Удаление записи и метод exclude">`  
    `![Удаление записи и метод exclude](screenshots/12_delete_and_exclude.png)`

13. **Выполнение индивидуального задания (Вариант 6 — модель `Review` и фильтр `rating__gt=3`):**  
    `<img src="screenshots/13_variant6_review_crud.png" alt="Индивидуальное задание Вариант 6">`  
    `![Индивидуальное задание Вариант 6](screenshots/13_variant6_review_crud.png)`

---

## Контрольные вопросы

### 1. Что такое модель в Django и для чего она используется?
Модель — это класс Python, наследуемый от `django.db.models.Model`, который определяет структуру хранимых данных и связывает код приложения с таблицей в базе данных. Каждая модель соответствует одной таблице, а её атрибуты — полям этой таблицы.

### 2. Какие типы полей вы знаете? Приведите примеры.
* `CharField(max_length=150)` — строковое поле ограниченной длины (например, заголовок).
* `TextField()` — текстовое поле неограниченной длины (контент статьи, текст отзыва).
* `IntegerField()` / `FloatField()` — целочисленные значения и числа с плавающей точкой.
* `DateTimeField()` — дата и время (с параметрами `auto_now_add` или `auto_now`).
* `BooleanField()` — логическое значение (`True` / `False`).
* `ImageField()` — поле для хранения пути к загруженному изображению.

### 3. Чем отличаются параметры `blank=True` и `null=True`?
`null=True` работает на уровне **базы данных** и разрешает хранить пустое значение `NULL` в столбце таблицы. `blank=True` работает на уровне **валидации форм Django** (например, в админ-панели) и разрешает оставлять поле пустым при отправке формы.

### 4. Что такое миграции и зачем они нужны? Опишите основные команды для работы с ними.
Миграции — это система контроля версий схемы базы данных в Django, позволяющая переносить изменения из кода моделей в таблицы БД.
* `python manage.py makemigrations` — создаёт Python-файлы миграций на основе изменений в `models.py`.
* `python manage.py migrate` — применяет созданные миграции к базе данных.
* `python manage.py sqlmigrate <app> <migration_id>` — выводит SQL-инструкции, которые формирует миграция.

### 5. Как создать суперпользователя для доступа к административной панели Django?
Необходимо выполнить в терминале команду `python manage.py createsuperuser`, после чего ввести имя пользователя, адрес электронной почты и пароль.

### 6. Что такое ORM? Перечислите основные методы для выполнения операций CRUD.
**ORM (Object-Relational Mapping)** — технология объектно-реляционного отображения, позволяющая работать с таблицами БД через объекты Python без написания прямых SQL-запросов:
* **Create:** `Model(field=val).save()` или `Model.objects.create(field=val)`
* **Read:** `Model.objects.all()`, `filter()`, `get()`, `exclude()`
* **Update:** изменение атрибута объекта `obj.field = new_val` и вызов `obj.save()`
* **Delete:** вызов метода `obj.delete()`

### 7. В чем разница между методами `filter()` и `get()`?
Метод `filter()` всегда возвращает коллекцию объектов `QuerySet` (даже если найдена одна запись или ни одной — пустой `QuerySet`). Метод `get()` возвращает ровно **один** объект модели и выбрасывает исключение `DoesNotExist`, если запись не найдена, или `MultipleObjectsReturned`, если условию удовлетворяет более одной записи.

### 8. Как настроить загрузку изображений (медиафайлов) в проекте Django?
1. Установить библиотеку `Pillow` (`pip install Pillow`).
2. Прописать в `settings.py` константы `MEDIA_ROOT = os.path.join(BASE_DIR, 'media')` и `MEDIA_URL = '/media/'`.
3. В корневом `urls.py` для режима `settings.DEBUG` добавить маршрут через `static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)`.

### 9. Для чего нужен метод `__str__` в модели?
Он определяет человекочитаемое строковое представление экземпляра модели (например, возвращает `self.title` вместо стандартного `<News: News object (1)>`) при выводе объектов в консоли `shell` и в административной панели Django.

### 10. Что произойдёт, если выполнить `python manage.py makemigrations`, но не выполнить `migrate`?
В папке `migrations` приложения появится файл с описанием изменений, однако в самой базе данных таблицы или столбцы созданы не будут. При попытке обратиться к такой модели через ORM возникнет ошибка `OperationalError: no such table`.

---

## Вывод
В ходе выполнения лабораторной работы №2 была изучена архитектура моделей и работа ORM в Django. Была разработана модель новостей `News`, настроена работа с медиафайлами (`Pillow`, `MEDIA_ROOT`, `MEDIA_URL`) и проведены миграции базы данных SQLite с проверкой DDL-схемы в менеджере SQLiteStudio. Через интерактивную консоль `shell` отработаны все операции CRUD (`create`, `save`, `all`, `filter`, `get`, `exclude`, `delete`). Дополнительно в рамках индивидуального задания (Вариант 6) реализована модель отзывов `Review` и выполнена фильтрация записей по условию `rating__gt=3`.

---

## Список использованных источников
1. **Модели Django (официальная документация)** — [https://docs.djangoproject.com/ru/5.0/topics/db/models/](https://docs.djangoproject.com/ru/5.0/topics/db/models/)
2. **Создание запросов в Django ORM (QuerySet API)** — [https://docs.djangoproject.com/ru/5.0/topics/db/queries/](https://docs.djangoproject.com/ru/5.0/topics/db/queries/)
3. **Миграции в Django** — [https://docs.djangoproject.com/ru/5.0/topics/migrations/](https://docs.djangoproject.com/ru/5.0/topics/migrations/)
4. **SQLiteStudio — менеджер баз данных SQLite** — [https://sqlitestudio.pl/](https://sqlitestudio.pl/)