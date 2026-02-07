# Captchat: Focus-to-Reveal Security CAPTCHA

![ezgif-52788b1f66b48f](https://github.com/user-attachments/assets/cd59098c-9362-427f-a3b1-528eb0474e4b)

Captchat is an advanced, two-factor CAPTCHA system that moves beyond simple static image recognition. It leverages the power of C++ (nodepp and Raylib) to dynamically generate secure image assets and couples them with an interactive, focus-based puzzle, enforced by robust server-side security.

# Unique Features

### 1. Focus-to-Reveal Puzzle
The core innovation is the visual puzzle mechanism. The full CAPTCHA image is hidden within a circular viewport. When a user focuses (clicks or tabs) on an input field, a dynamic CSS rule is activated:
The image within the viewport zooms in (6x) and pans to reveal a single, specific character of the solution.
The entire viewport rotates slightly for visual obfuscation.
This process must be repeated for all six input fields to reveal the full, scrambled solution.

### 2. Base64 CSS Asset Injection
Images are generated using Raylib in the C++ environment, converted to Base64-encoded PNGs, and injected directly into a .css file using attribute selectors (e.g., [img="0"]).
No File Requests: The images are delivered via a single CSS stylesheet, minimizing HTTP requests.
Dynamic Generation: The assets are fresh, randomized, and tailored to the running environment.

### 3. Advanced Server-Side Security
The server logic employs sophisticated defense mechanisms:
JWT Session Tracking: A JSON Web Token (JWT) containing a random seed (tkn) and an expiration timestamp is issued with the challenge, preventing replay attacks.
Scrambled Solution Derivation: The required character for each input box is not simply the character at a fixed position. It is cryptographically derived using an arithmetic formula that incorporates bytes from the initial JWT seed (tkn) and the full CAPTCHA solution string. The required solution changes based on the user's session token.

# Read How it works:
[![IMAGE](https://miro.medium.com/v2/resize:fit:720/format:webp/1*rSM0btAweNwnDFWvxblYfQ.png)](https://medium.com/@EDBCBlog/surviving-the-shadows-creating-a-captchat-without-javascript-1bdda8435fc3])

[Click here](https://medium.com/@EDBCBlog/surviving-the-shadows-creating-a-captchat-without-javascript-1bdda8435fc3)
