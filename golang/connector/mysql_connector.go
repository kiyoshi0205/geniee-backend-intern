package connector

import (
	"auction-worker/config"
	"fmt"
	"reflect"
	"sync"

	"github.com/jmoiron/sqlx"
)

// MySQLに接続しデータを取得する関数群が定義されているクラス。スレッド間で共有したいものがあればこちらを使用可能
type MySQLConnector struct {
	mut sync.Mutex
}

// SQLクエリを実行しデータ全てを返す関数。modelsに配列に結果を格納。DBに接続できないときやデータが取得できないときにエラーを返す。
func (mysql_connector *MySQLConnector) fetchModels(query string, models any) error {
	mysql_connector.mut.Lock()
	defer mysql_connector.mut.Unlock()

	value := reflect.ValueOf(models)
	if value.Kind() != reflect.Ptr || value.Elem().Kind() != reflect.Slice {
		// 型の間違いは復旧不可能なのでpanic
		panic("models argument must be a pointer of slice (*[]any)")
	}

	db, err := sqlx.Open("mysql", config.Dns)
	if err != nil {
		return err
	}
	defer db.Close()
	if err = db.Ping(); err != nil {
		return nil
	}

	err = db.Select(models, query)
	if err != nil {
		return err
	}
	// 該当するデータが一つもない場合、この関数はerrを返す
	if value.Elem().Len() == 0 {
		return fmt.Errorf("no data found on mysql server")
	}
	return nil
}

// SQLクエリを実行し指定したIDデータを返す関数、modelsに配列に結果を格納。DBに接続できないときやデータが取得できないときにエラーを返す。
func (mysql_connector *MySQLConnector) fetchModelByID(query string, id uint, model any) error {
	mysql_connector.mut.Lock()
	defer mysql_connector.mut.Unlock()

	value := reflect.ValueOf(model)
	if value.Kind() != reflect.Ptr {
		// 型の間違いは復旧不可能なのでpanic
		panic("model argument must be a pointer (*any)")
	}

	db, err := sqlx.Open("mysql", config.Dns)
	if err != nil {
		return err
	}
	defer db.Close()
	if err = db.Ping(); err != nil {
		return nil
	}

	if value.Elem().Kind() == reflect.Slice {
		// IDに紐づく値が複数ある場合。
		err = db.Select(model, query, id)
		if value.Elem().Len() == 0 {
			// 該当するデータが一つもない場合、この関数はerrを返す
			return fmt.Errorf("no data found on mysql server")
		}
	} else {
		// 該当するデータが一つもない場合、この関数はerrを返す
		err = db.Get(model, query, id)
	}

	if err != nil {
		return err
	}
	return nil
}
