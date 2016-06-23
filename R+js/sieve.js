function Filter(number) {
    this.number = number;
    this.next = null;
    this.last = this;
}
Filter.prototype.acceptAndAdd = function(n) {
  var filter = this;
  var sqrt = Math.sqrt(n);
  for (;;) {
      if (n % filter.number === 0) {
          return false;
      }
      if (filter.number > sqrt) {
          break;
      }
      filter = filter.next;
  }
  var newFilter = new Filter(n);
  this.last.next = newFilter;
  this.last = newFilter;
  return true;
};

function Primes(natural) {
    this.natural = natural;
    this.at = 0;
    this.filter = null;
}
Primes.prototype.next = function() {
    for (;;) {
        var n = this.natural[this.at++];
        if (this.filter === null) {
            this.filter = new Filter(n);
            return n;
        }
        if (this.filter.acceptAndAdd(n)) {
            return n;
        }
    }
};

function measure(prntCnt, upto) {
    var natural = Interop.import('Natural');
    var primes = new Primes(Interop.execute(natural));

    var log = typeof console !== 'undefined' ? console.log : print;
    var start = new Date().getTime();
    var cnt = 0;
    var res = -1;
    for (;;) {
        res = primes.next();
        cnt++;
        if (cnt % prntCnt === 0) {
            log("Computed " + cnt + " primes in " + (new Date().getTime() - start) + " ms. Last one is " + res);
            prntCnt *= 2;
        }
        if (upto && cnt >= upto) {
            break;
        }
    }
    return new Date().getTime() - start;
}

for (;;) {
    var log = typeof console !== 'undefined' ? console.log : print;
    log("Hundred thousand prime numbers in " + measure(97, 100000) + " ms");
}
