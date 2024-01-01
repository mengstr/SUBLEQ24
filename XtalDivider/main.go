package main

import (
	"fmt"
)

const slotmin = 2

// The main function
func main() {
	best := ""
	bestCount := 0
	bestPattern := ""

	var slots [100]int

	numbers := []float64{
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
		51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
		// 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
	}

	for i1 := 0; i1 < len(numbers); i1++ {
		number1 := numbers[i1]
		for i2 := i1 + 1; i2 < len(numbers); i2++ {
			number2 := numbers[i2]
			for i3 := i2 + 1; i3 < len(numbers); i3++ {
				number3 := numbers[i3]
				for i4 := i3 + 1; i4 < len(numbers); i4++ {
					number4 := numbers[i4]

					// zero out the slots
					for i := 0; i < 100; i++ {
						slots[i] = 0
					}

					for div := 1; div < 17; div++ {
						freq := number1 / float64(div)
						freq = freq / 2
						if freq >= 0.1 && freq < 10.0 {
							slots[int(freq*10)]++
							// fmt.Printf("N1: %f\n", freq)
						}
					}
					for div := 1; div < 17; div++ {
						freq := number2 / float64(div)
						freq = freq / 2
						if freq >= 0.1 && freq < 10.0 {
							slots[int(freq*10)]++
							// fmt.Printf("N2: %f\n", freq)
						}
					}
					for div := 1; div < 17; div++ {
						freq := number3 / float64(div)
						freq = freq / 2
						if freq >= 0.1 && freq < 10.0 {
							slots[int(freq*10)]++
							// fmt.Printf("N3: %f\n", freq)
						}
					}
					for div := 1; div < 17; div++ {
						freq := number4 / float64(div)
						freq = freq / 2
						if freq >= 0.1 && freq < 10.0 {
							slots[int(freq*10)]++
							// fmt.Printf("N4: %f\n", freq)
						}
					}

					count := 0
					pattern := ""
					for i := 0; i < 100; i++ {
						if slots[i] > 0 {
							pattern += "X"
							count++
						} else {
							pattern += "-"
						}
						if i%10 == 9 {
							pattern += " "
						}
					}

					fmt.Printf("%s\n", pattern)
					decade := 0
					for i := 0; i < 100; i += 10 {
						if slots[i+0]+slots[i+1]+slots[i+2]+slots[i+3]+slots[i+4]+slots[i+5]+slots[i+6]+slots[i+7]+slots[i+8]+slots[i+9] >= slotmin {
							decade++
						}
						if count > bestCount && decade == 10 {
							bestCount = count
							bestPattern = pattern
							best = fmt.Sprintf("%f %f %f %f", number1, number2, number3, number4)
							fmt.Printf("\nBest so far with %d slots: %s\n%s\n", bestCount, best, bestPattern)
						}
					}
				}
			}
		}
	}
	fmt.Printf("\nAbsolute Best with %d slots: %s\n%s\n", bestCount, best, bestPattern)
}
