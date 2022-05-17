package main

import (
	"flag"
	"io"
	"log"
	"net/http"
	"strconv"
	"strings"

	"github.com/gin-gonic/gin"
	"github.com/tarm/serial"
)

var (
	Addr = flag.String("addr", ":8080", "listen address")
	Dev  = flag.String("dev", "/dev/ttyUSB0", "serial device")
	Baud = flag.Int("baud", 9600, "serial baud rate")
)

var (
	Serial *serial.Port
	Conf   Config
)

type Config struct {
	Brightness int
	Speed      int
}

func init() { flag.Parse() }

func init() {
	var err error
	c := &serial.Config{
		Name: *Dev,
		Baud: *Baud,
	}
	Serial, err = serial.OpenPort(c)
	if err != nil {
		panic(err.Error())
	}
}

func init() { gin.SetMode(gin.ReleaseMode) }

func main() {

	// serial
	go func() {
		defer Serial.Close()
		b := make([]byte, 128)
		for {
			n, err := Serial.Read(b)
			if err == io.EOF {
				break
			}
			if err != nil {
				panic(err.Error())
			}
			if a := strings.Split(string(b[:n]), ","); len(a) == 2 {
				Conf.Brightness, _ = strconv.Atoi(a[0])
				Conf.Speed, _ = strconv.Atoi(a[1])
			}
		}
	}()

	// http
	r := gin.New()

	r.POST("/brightness", func(c *gin.Context) {
		s := c.PostForm("val")
		v, err := strconv.Atoi(s)
		if err != nil {
			c.Status(http.StatusBadRequest)
			return
		}
		Conf.Brightness = v
		log.Print("new brightness val: ", v)
	})

	r.GET("/brightness", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(Conf.Brightness))
	})

	r.POST("/speed", func(c *gin.Context) {
		s := c.PostForm("val")
		v, err := strconv.Atoi(s)
		if err != nil {
			c.Status(http.StatusBadRequest)
			return
		}
		Conf.Speed = v
		log.Print("new speed val: ", v)
	})

	r.GET("/speed", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(Conf.Speed))
	})

	r.Run(*Addr)
}
