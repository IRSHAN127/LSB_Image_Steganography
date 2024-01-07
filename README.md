<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
<body>

<h1>Project 1: LSB Image Steganography</h1>
<h3>Written by: MD IRSHAN NAZIR</h3>
<h3 align="left">Connect with me:</h3>
<p align="left">
<a href="https://www.linkedin.com/in/irshan127/" target="blank"><img align="center" src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/linked-in-alt.svg" alt="https://www.linkedin.com/in/irshan127/" height="30" width="40" /></a> <a href="https://twitter.com/Irshan_127" target="blank"><img align="center" src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/twitter.svg" alt="https://twitter.com/irshan_127" height="35" width="40" />
</p>

<h2>Abstract:</h2>

<p>Steganography is the art of hiding the fact that communication is taking place, by hiding information in other information.
Many different carrier file formats can be used, but digital images are the most popular because of their frequency on the internet.
For hiding secret information in images, there exists a large variety of steganography techniques, some more complex than others,
each with respective strong and weak points. Different applications may require absolute invisibility of the secret information,
while others require a large secret message to be hidden.</p>

<p>Steganography is the practice of hiding private or sensitive information within something that appears to be nothing out of the usual.
Steganography is often confused with cryptology because the two are similar in the way that they both are used to protect important information.
The difference between the two is that steganography involves hiding information so it appears that no information is hidden at all.
If a person views the object that the information is hidden inside, he or she will have no idea that there is any hidden information,
and therefore, the person will not attempt to decrypt the information.</p>

<p>What steganography essentially does is exploit human perception; human senses are not trained to look for files that have information inside them,
although software is available that can perform steganography. The most common use of steganography is to hide a file inside another file.</p>

<h2>Requirements:</h2>

<ul>
  <li>The application should accept an image file, say .bmp, along with a text file containing the message to be stegged.</li>
  <li>Analyze the size of the message file and the data part of the .bmp file to check whether the message could fit in the provided .bmp image.</li>
  <li>Provide an option to steg a magic string that could be useful to identify whether the image is steged or not.</li>
  <li>The application should provide an option to decrypt the file.</li>
  <li>This has to be a command-line application, and all the options have to be passed as command-line arguments.</li>
</ul>

<h2>Pre-requisite:</h2>

<ul>
  <li>Knowledge of encryption and decryption.</li>
  <li>File I/O.</li>
  <li>Pointers and Structures.</li>
</ul>

<p>This LSB IMAGE STEGANOGRAPHY project is completed in C language. In this project, we can hide a .txt file message in a bmp file and generate a stego.bmp file.
This stego.bmp file is used to decode the secret.txt message. To run the code, compile it using <code>gcc *.c</code> (to compile all .c files at once) or
<code>gcc</code> (followed by all .c <filename>). After compilation, the <code>a.out</code> file will be generated, then execute it by:</p>

<h3>For Encoding (-e)</h3>
<pre>
  <code>./a.out -e secret.txt beautiful.bmp</code>
  or
  <code>./a.out -e secret.txt beautiful.bmp stego.bmp</code>
</pre>

<h3>For Decoding (-d)</h3>
<pre>
  <code>./a.out -d stego.bmp</code>
  or
  <code>./a.out -d stego.bmp decoded_secret.txt</code>
</pre>

<h2>References:</h2>

<ul>
  <li><a href="https://en.wikipedia.org/wiki/Steganography">Steganography</a></li>
  <li><a href="https://en.wikipedia.org/wiki/BMP">BMP</a></li>
</ul>

</body>
</html>
