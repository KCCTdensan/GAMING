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
	Type int
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
			if a := strings.Split(s[:len(s)-2], ","); len(a) == 2 {
				v0, err := strconv.Atoi(a[0])
				if err != nil {
					continue
				}
				v1, err := strconv.Atoi(a[1])
				if err != nil {
					continue
				}
				conf.Angle = v0
				conf.Delay = v1
			}
		}
	}()
	s.Write([]byte("D"))

	// http
	r := gin.New()

	r.GET("/angle", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(conf.Angle))
	})
	r.GET("/type", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(conf.Type))
	})
	r.GET("/delay", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(conf.Delay))
	})

	r.POST("/suspend", func(c *gin.Context) {
		s.Write([]byte("S"))
		log.Print("Suspend")
	})
	r.POST("/reset", func(c *gin.Context) {
		s.Write([]byte("R"))
		log.Print("Reset")
	})

	r.POST("/angle", func(c *gin.Context) {
		v := c.PostForm("val")
		n, err := strconv.Atoi(v)
		if err != nil {
			c.Status(http.StatusBadRequest)
			return
		}
		conf.Angle = n
		s.Write([]byte(fmt.Sprintf("a %d", n)))
		log.Printf("Angle = %d", n)
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
		log.Printf("Delay = %d", n)
	})
	r.POST("/type", func(c *gin.Context) {
		v := c.PostForm("val")
		n, err := strconv.Atoi(v)
		if err != nil {
			c.Status(http.StatusBadRequest)
			return
		}
		conf.Type = n
		s.Write([]byte(fmt.Sprintf("c %d", n)))
		log.Printf("Type = %d", n)
	})

	r.Run(*Addr)
}
