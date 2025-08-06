window.addEventListener("load", () => {
  console.log("js works")


  const send = document.querySelector(".send")
  const textarea = document.querySelector(".query")
  const resp = document.querySelector("#response")
  send.addEventListener("click", () => {
    (async () => {
      try {
        const data = await fetch("http://localhost:8080/chat", {
          method: "POST",
          headers: {
            "Content-Type": "application/json"
          },
          body: JSON.stringify({ body: textarea.value.trim() })
        })


        const res= await data.json()

        resp.textContent =res.response
        console.log(res.response)

      } catch (e) { console.log(e) }
    })();
  })


})


