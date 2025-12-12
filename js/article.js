const params = new URLSearchParams(window.location.search);
const slug = params.get("slug");

const content = document.getElementById("content");

if (!slug) {
  content.innerHTML = "<p>Missing article slug.</p>";
  throw new Error("No slug");
}

fetch(`${slug}.md`)
  .then(res => {
    if (!res.ok) throw new Error("Markdown not found");
    return res.text();
  })
  .then(md => {
    content.innerHTML = marked.parse(md);
  })
  .catch(err => {
    content.innerHTML = "<p>Article not found.</p>";
    console.error(err);
  });
