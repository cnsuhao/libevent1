# in __init.py__
import time
import databaseHelper
db  = databaseHelper.SqliteHelper("data.db")
db._connect()
def error():
    return _db.last_error
class notebook(object):
    @staticmethod
    def __init__():
        try:
            db.execute("""CREATE TABLE notebook (id INTEGER PRIMARY KEY,name TEXT NOT NULL)""")
        except:
            pass
    def new(name,idin=None):
        return db.execute("INSERT INTO notebook VALUES(?,?)", (idin,name))
    @staticmethod
    def delete(id):
        return db.execute("DELETE FROM notebook WHERE id=?", (id,))
    @staticmethod
    def update():
        return db.commit()
    @staticmethod
    def get_all():
        return db.query_all("SELECT * FROM notebook ORDER BY id ASC")
class chapter(object):
    @staticmethod
    def new(name, notebook_id=1):
        return db.execute("INSERT INTO chapter VALUES(null,?,?)", (name, notebook_id))
    @staticmethod
    def delete(id):
        return db.execute("DELETE FROM chapter WHERE id=?", (id,))
    @staticmethod
    def update(id, name):
        return db.execute("UPDATE chapter SET name=? WHERE id=?", (name, id))
    @staticmethod
    def get_all(notebook_id=None):
        if notebook_id:
            return db.query_all("SELECT * FROM chapter ORDER BY id ASC WHERE nid=?", (notebook_id,))
        else:
            return db.query_all("SELECT * FROM chapter ORDER BY id ASC")
if __name__ == "__main__":
    print(notebook.new('Math',1000))
    db.update()
    print(notebook.get_all())
