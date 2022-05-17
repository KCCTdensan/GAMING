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

type Config struct {
	Brightness int
	Speed      int
}

func init() { flag.Parse() }

func init() { gin.SetMode(gin.ReleaseMode) }

func main() {
	var conf Config

	// serial
	c := &serial.Config{
		Name: *Dev,
		Baud: *Baud,
	}
	s, err := serial.OpenPort(c)
	if err != nil {
		panic(err.Error())
	}
	ch := make(chan Config)
	go listenSerial(s, ch)

	// http
	r := gin.New()

	r.POST("/brightness", func(c *gin.Context) {
		s := c.PostForm("val")
		v, err := strconv.Atoi(s)
		if err != nil {
			c.Status(http.StatusBadRequest)
			return
		}
		conf.Brightness = v
		log.Print("new brightness val: ", v)
	})

	r.GET("/brightness", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(conf.Brightness))
	})

	r.POST("/speed", func(c *gin.Context) {
		s := c.PostForm("val")
		v, err := strconv.Atoi(s)
		if err != nil {
			c.Status(http.StatusBadRequest)
			return
		}
		conf.Speed = v
		log.Print("new speed val: ", v)
	})

	r.GET("/speed", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(conf.Speed))
	})

	r.Run(*Addr)
}

func listenSerial(s *serial.Port, c chan Config) {
	defer s.Close()
	b := make([]byte, 128)
	for {
		n, err := s.Read(b)
		if err == io.EOF {
			break
		}
		if err != nil {
			panic(err.Error())
		}
		if a := strings.Split(string(b[:n]), ","); len(a) == 2 {
			brightness, err := strconv.Atoi(a[0])
			if err != nil {
				continue
			}
			speed, err := strconv.Atoi(a[1])
			if err != nil {
				continue
			}
			c <- Config{brightness, speed}
		}
	}
}
