#!/usr/bin/env python
# -*- coding: utf-8 -*-
__author__ = 'draco'
import sqlite3
class SqliteHelper(object):  
    """
    """
    @property
    def last_error(self):
        return self._last_error
    def __init__(self, db=":memory:"):
        super(SqliteHelper, self).__init__()
        self._last_error = None
        self._db = db
        self._con = None
    def config(self, db):
        self._db = db
        self._con = None
    def update(self):
        self._con.commit()
    def _connect(self):
        self.reset_error()
        self._con = sqlite3.connect(self._db)
        # foreign key support
        self._con.execute("pragma foreign_keys = on")
        return self._con.cursor()
    def on_error(self, err):
        self._last_error = err
    def reset_error(self):
        self._last_error = None
    def execute(self, sql, param):
        lid = 0
        cur = self._connect()
        with self._con:
            try:
                if param:
                    cur.execute(sql, param)
                else:
                    cur.execute(sql)
                self._con.commit()
                if cur.lastrowid is not None:
                    lid = cur.lastrowid
                self.reset_error()
            except sqlite3.Error as e:
                if self._con:
                    self._con.rollback()
                self.on_error(e)
            finally:
                return lid
    def get_raw(self, sql, param=None):
        cur = self._connect()
        data = None
        with self._con:
            try:
                if param:
                    cur.execute(sql, param)
                else:
                    cur.execute(sql)
                data = cur.fetchone()[0]
                self.reset_error()
            except sqlite3.Error as e:
                if self._con:
                    self._con.rollback()
                self.on_error(e)
            finally:
                return data
    def query(self, sql, param=None):
        cur = self._connect()
        data = []
        with self._con:
            try:
                if param:
                    cur.execute(sql, param)
                else:
                    cur.execute(sql)
                row = cur.fetchone()
                # format data
                if row:
                    field_names = [f[0] for f in cur.description]
                    data = dict(zip(field_names, row))
                self.reset_error()
            except sqlite3.Error as e:
                if self._con:
                    self._con.rollback()
                self.on_error(e)
            finally:
                return data
    def query_all(self, sql, param=None):
        cur = self._connect()
        data = []
        with self._con:
            try:
                if param:
                    cur.execute(sql, param)
                else:
                    cur.execute(sql)
                rows = cur.fetchall()
                # format data
                if len(rows) > 0:
                    field_names = [f[0] for f in cur.description]
                    data = [dict(zip(field_names, r)) for r in rows]
                self.reset_error()
            except sqlite3.Error as e:
                if self._con:
                    self._con.rollback()
                self.on_error(e)
            finally:
                return data
