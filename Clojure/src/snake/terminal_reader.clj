; Copyright (C) 2008 - 2011 Stefan Marr <mail@stefan-marr.de>
; 
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to
; deal in the Software without restriction, including without limitation the
; rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
; sell copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
; 
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
; 
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
; FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
; IN THE SOFTWARE.

(ns snake.terminal-reader
  (:use snake.input-receiver))

(defn get-input
  "Reads a string from the given input-reader if there is anything to be read"
  [input-reader]
  
  (let [buffer (char-array 1024)]
    (if (.ready input-reader)
      (let [num-chars-read (.read input-reader buffer)]
        (if (= num-chars-read -1)
          ""
          (String/valueOf buffer 0 num-chars-read)))
      "")))


(defn read-input
  "Read from standard input and decode the input into symbols
   to be understoud be the input-receiver."
  [input-receiver input-reader]
  (loop [data (get-input input-reader)]
    (case data
      "\u001B[A" (send input-receiver input-direction :up   )
      "\u001B[B" (send input-receiver input-direction :down )
      "\u001B[D" (send input-receiver input-direction :left )
      "\u001B[C" (send input-receiver input-direction :right)
      :nothing)
    (. Thread sleep 80)  ;; necessary since get-input is non-blocking
    (recur (get-input input-reader))))

(defn start-terminal-reader
  "Starts a new thread that is reading from standard in and will
   notify an agent about all read input."
  [input-receiver]
  (let [input-reader (new java.io.InputStreamReader System/in)]
    (.start (Thread. (fn [] (read-input input-receiver input-reader))))))
