package main

import (
	"flag"
	"fmt"
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
	Baud = flag.Int("baud", 115200, "serial baud rate")
)

type Config struct {
	Angle int
	Delay int
}

func listenSerial(s *serial.Port, c chan string) {
	b := make([]byte, 128)
	for {
		n, err := s.Read(b)
		if err == io.EOF {
			break
		}
		if err != nil {
			panic(err.Error())
		}
		c <- string(b[:n])
	}
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
	defer s.Close()
	ch := make(chan string)
	go listenSerial(s, ch)
	go func() {
		// 仮
		for s := range ch {
			if a := strings.Split(s, ","); len(a) == 2 {
				v0, err := strconv.Atoi(a[0])
				if err != nil {
					continue
				}
				v1, err := strconv.Atoi(a[1])
				if err != nil {
					continue
				}
				conf = Config{v0, v1}
			}
		}
	}()

	// http
	r := gin.New()

	r.POST("/angle", func(c *gin.Context) {
		v := c.PostForm("val")
		n, err := strconv.Atoi(v)
		if err != nil {
			c.Status(http.StatusBadRequest)
			return
		}
		conf.Angle = n
		s.Write([]byte(fmt.Sprintf("a %d", n)))
		log.Print("new brightness val: ", n)
	})

	r.GET("/angle", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(conf.Angle))
	})

	r.POST("/delay", func(c *gin.Context) {
		v := c.PostForm("val")
		n, err := strconv.Atoi(v)
		if err != nil {
			c.Status(http.StatusBadRequest)
			return
		}
		conf.Delay = n
		s.Write([]byte(fmt.Sprintf("d %d", n)))
		log.Print("new delay val: ", n)
	})

	r.GET("/delay", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(conf.Delay))
	})

	r.POST("/reset", func(c *gin.Context) {
		s.Write([]byte("R"))
	})

	r.Run(*Addr)
}
