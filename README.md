# Custom SQL Parser

### Run

```bash
g++ -o app main.cpp
./app
```

### Keywords

All keywords are case insensative

```SQL
SELECT, DELETE, ADD, WHERE
```

#### SELECT QUERY

```SQL
SELECT [tableName] WHERE *
SELECT [tableName] WHERE [field] = [value]
SELECT [tableName] WHERE [field] != [value]
```

#### DELETE QUERY

```SQL
DELETE [tableName] WHERE *
DELETE [tableName] WHERE [field] = [value]
DELETE [tableName] WHERE [field] != [value]
```

#### ADD QUERY

```SQL
ADD [tableName]
```
