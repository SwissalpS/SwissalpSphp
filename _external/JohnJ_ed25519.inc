<?php
/* * * *
 * * JohnJ_ed25519.inc
 * * A PHP implementation of the Python ED25519 library by John Judy
 * *
 * * SwissalpS grabbed this from:
 * * https://github.com/trianglman/sqrl/
 * * and only made slight changes like removing namespace and changing
 * * the class' name from Crypto to JohnJ_ed25519. Also the occasional
 * * white-space was added or changed for readability
 * *
 * * @version 20131107_163303 (CC) Luke JZ aka SwissalpS
 * * * */
/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 John Judy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
ini_set('xdebug.max_nesting_level', 0);
/**
 * A PHP implementation of the Python ED25519 library
 *
 * @author johnj
 *
 * @link http://ed25519.cr.yp.to/software.html Other ED25519 implementations this is referenced from
 */
class JohnJ_ed25519 {

    protected $b;
    protected $q;
    protected $l;
    protected $d;
    protected $I;
    protected $By;
    protected $Bx;
    protected $B;

    public function __construct() {

        $this->b = 256;
        $this->q = "57896044618658097711785492504343953926634992332820282019728792003956564819949";//bcsub(bcpow(2, 255),19);
        $this->l = "7237005577332262213973186563042994240857116359379907606001950938285454250989";//bcadd(bcpow(2,252),27742317777372353535851937790883648493);
        $this->d = "-4513249062541557337682894930092624173785641285191125241628941591882900924598840740";//bcmul(-121665,$this->inv(121666));
        $this->I = "19681161376707505956807079304988542015446066515923890162744021073123829784752";//$this->expmod(2,  bcdiv((bcsub($this->q,1)),4),$this->q);
        $this->By = "46316835694926478169428394003475163141307993866256225615783033603165251855960";//bcmul(4,$this->inv(5));
        $this->Bx = "15112221349535400772501151409588531511454012693041857206046113283949847762202";//$this->xrecover($this->By);
        $this->B = array("15112221349535400772501151409588531511454012693041857206046113283949847762202", "46316835694926478169428394003475163141307993866256225615783033603165251855960");//array(bcmod($this->Bx,$this->q),bcmod($this->By,$this->q));

    } // __construct

    protected function H($m) { return hash('sha512', $m, true); } // H

    //((n % M) + M) % M //python modulus craziness
    protected function pymod($x, $m) {

        $mod = bcmod($x, $m);
        if ($mod[0] === '-') {
            $mod = bcadd($mod, $m);
        }

        return $mod;

    } // pymod

    protected function expmod($b, $e, $m) {

        if ($e == 0) { return 1; }
        $t = bcpowmod($b, $e, $m);
        if ($t[0] === '-') {
            $t = bcadd($t, $m);
        }

        return $t;

    } // expmod

    protected function inv($x) {

        return $this->expmod($x, bcsub($this->q, 2), $this->q);

    } // inv

    protected function xrecover($y) {

        $y2 = bcpow($y, 2);
        $xx = bcmul(bcsub($y2, 1), $this->inv(bcadd(bcmul($this->d, $y2), 1)));
        $x = $this->expmod($xx, bcdiv(bcadd($this->q, 3), 8, 0), $this->q);
        if ($this->pymod(bcsub(bcpow($x, 2) , $xx), $this->q) != 0) { $x = $this->pymod(bcmul($x, $this->I), $this->q); }
        if (substr($x, -1) % 2 != 0) { $x= bcsub($this->q, $x); }

        return $x;

    } // xrecover

    protected function edwards($P, $Q) {

        list($x1, $y1) = $P;
        list($x2, $y2) = $Q;
        $com = bcmul($this->d, bcmul(bcmul($x1, $x2), bcmul($y1, $y2)));
        $xl = bcadd(bcmul($x1, $y2), bcmul($x2, $y1));
        $xr = $this->inv(bcadd(1, $com));
        $x3 = bcmul($xl, $xr);
        $yl = bcadd(bcmul($y1, $y2), bcmul($x1, $x2));
        $yr = $this->inv(bcsub(1, $com));
        $y3 = bcmul($yl, $yr);

        return array($this->pymod($x3, $this->q), $this->pymod($y3, $this->q));

    } // edwards

    protected function scalarmult($P, $e) {

        if ($e == 0) { return array(0, 1); }
        $Q = $this->scalarmult($P, bcdiv($e, 2, 0));
        $Q = $this->edwards($Q, $Q);
        if (substr($e, -1) % 2 == 1){
            $Q = $this->edwards($Q, $P);
        }

        return $Q;

    } // scalarmult

    protected function bitsToString($bits) {

        $string = '';
        for ($i = 0; $i < $this->b / 8; $i++) {

            $sum = 0;
            for ($j = 0; $j < 8; $j++) {

                $bit = $bits[$i * 8 + $j];
                $sum += (int)$bit << $j;

            } // loop j

            $string .= chr($sum);

        } // loop i

        return $string;

    } // bitsToString

    protected function dec2bin_i($decimal_i) {

        $binary_i = '';
        do {

            $binary_i = substr($decimal_i, -1) % 2 . $binary_i;
            $decimal_i = bcdiv($decimal_i, '2', 0);

         } while (bccomp($decimal_i, '0'));

        return($binary_i);

    } // dec2bin_i

    protected function encodeint($y) {

        $bits = substr(str_pad(strrev($this->dec2bin_i($y)), $this->b, '0', STR_PAD_RIGHT), 0, $this->b);

        return $this->bitsToString($bits);

    } // encodeint

    protected function encodepoint($P) {

        list($x, $y) = $P;
        $bits = substr(str_pad(strrev($this->dec2bin_i($y)), $this->b - 1, '0', STR_PAD_RIGHT), 0, $this->b - 1);
        $bits .= (substr($x, -1) % 2 == 1 ? '1' : '0');

        return $this->bitsToString($bits);

    } // encodepoint

    protected function bit($h, $i) {

        return (ord($h[(int)bcdiv($i, 8, 0)]) >> substr($i, -3) % 8) & 1;

    } // bit

    /**
     * Generates the public key of a given private key
     *
     * @param string $sk the secret key
     * @return string
     */
    public function publickey($sk) {

        $h = $this->H($sk);
        $sum = 0;
        for ($i = 3; $i < $this->b - 2; $i++) {

            $sum = bcadd($sum, bcmul(bcpow(2, $i), $this->bit($h, $i)));

        } // loop i

        $a = bcadd(bcpow(2, $this->b - 2), $sum);
        $A = $this->scalarmult($this->B, $a);
        $data = $this->encodepoint($A);

        return $data;

    } // publickey

    protected function Hint($m) {

        $h = $this->H($m);
        $sum = 0;
        for ($i = 0; $i < $this->b * 2; $i++) {

            $sum = bcadd($sum, bcmul(bcpow(2, $i), $this->bit($h, $i)));

        } // loop i

        return $sum;

    } // Hint

/**
 * def signature(m,sk,pk):
  h = H(sk)
  a = 2**(b-2) + sum(2**i * bit(h,i) for i in range(3,b-2))
 *
  r = Hint( ''.join( [h[i] for i in range(b/8,b/4)] ) + m )
  R = scalarmult(B,r)
  S = (r + Hint(encodepoint(R) + pk + m) * a) % l
  return encodepoint(R) + encodeint(S)

 */
    public function signature($m, $sk, $pk) {

        $h = $this->H($sk);
        $a = bcpow(2, (bcsub($this->b, 2)));
        for ($i = 3; $i < $this->b - 2; $i++) {

            $a = bcadd($a, bcmul(bcpow(2, $i), $this->bit($h, $i)));

        } // loop i

        $r = $this->Hint(substr($h, $this->b / 8, ($this->b / 4 - $this->b / 8)) . $m);
        $R = $this->scalarmult($this->B, $r);
        $encR = $this->encodepoint($R);
        $S = $this->pymod(bcadd($r, bcmul($this->Hint($encR . $pk . $m), $a)), $this->l);

        return $encR . $this->encodeint($S);

    } // signature

    protected function isoncurve($P) {

        list($x, $y) = $P;
        $x2 = bcpow($x, 2);
        $y2 = bcpow($y, 2);

        return $this->pymod(bcsub(bcsub(bcsub($y2, $x2), 1), bcmul($this->d, bcmul($x2, $y2))), $this->q) == 0;

    } // isoncurve

    protected function decodeint($s) {

        $sum = 0;
        for ($i = 0; $i < $this->b; $i++) {

            $sum = bcadd($sum, bcmul(bcpow(2, $i), $this->bit($s, $i)));

        } // loop i

        return $sum;

    } // decodeint

/*
 * def decodepoint(s):
  y = sum(2**i * bit(s,i) for i in range(0,b-1))
  x = xrecover(y)
  if x & 1 != bit(s,b-1): x = q-x
  P = [x,y]
  if not isoncurve(P): raise Exception("decoding point that is not on curve")
  return P

 */
    protected function decodepoint($s) {

        $y = 0;
        for ($i = 0; $i < $this->b - 1; $i++) {

            $y = bcadd($y, bcmul(bcpow(2, $i), $this->bit($s, $i)));

        } // loop i

        $x = $this->xrecover($y);
        if (substr($x, -1) % 2 != $this->bit($s, $this->b - 1)) {

            $x = bcsub($this->q, $x);

        } // if

        $P = array($x, $y);
        if (!$this->isoncurve($P)) {

            throw new Exception("Decoding point that is not on curve");

        } // if not on curve

        return $P;

    } // decodepoint

    public function checkvalid($s, $m, $pk) {

        if (strlen($s) != $this->b / 4) {

            throw new Exception('Signature length is wrong');

        } // if wrong sig length

        if (strlen($pk) != $this->b / 8) {

            throw new Exception('Public key length is wrong: ' . strlen($pk));

        } // if wrong pk length

        $R = $this->decodepoint(substr($s, 0, $this->b / 8));
        try {

            $A = $this->decodepoint($pk);

        } catch (Exception $e) { return false; }

        $S = $this->decodeint(substr($s, $this->b / 8, $this->b / 4));
        $h = $this->Hint($this->encodepoint($R) . $pk . $m);

        return $this->scalarmult($this->B, $S) == $this->edwards($R, $this->scalarmult($A, $h));

    } // checkvalid

} // JohnJ_ed25519

/* * * *\ JohnJ_ed25519.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
