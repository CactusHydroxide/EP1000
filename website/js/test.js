let arr = ['words', 10, true, { 'key': 'value' }]

let arrstr = JSON.stringify(arr)
console.log(arrstr)
console.log(typeof (arrstr))

let arrback = JSON.parse(arrstr)
console.log(arrback)
console.log(typeof (arrback))


console.log(arrback[0])
console.log(arrback[1])
console.log(arrback[2])
console.log(arrback[3])
console.log(typeof(arrback[3]))