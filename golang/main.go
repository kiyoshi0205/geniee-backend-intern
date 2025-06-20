package main

import (
	"auction-worker/connector"
	"auction-worker/server"
	"fmt"
	"os"

	_ "github.com/go-sql-driver/mysql"
)

func main() {
	pid := os.Getpid()
	fmt.Printf("pid = %d\n", pid)

	mysql_connector := connector.MySQLConnector{}
	srv := server.NewHttpServer()
	srv.SetupCb("/auction", server.AuctionHandler, &mysql_connector)
	srv.Run()
}
