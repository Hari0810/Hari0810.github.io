const params = new URLSearchParams(window.location.search);
const slug = params.get("slug");

if (!slug) {
  document.getElementById("content").innerHTML =
    "<p class='text-red-600'>No article specified.</p>";
  throw new Error("Missing slug");
}

fetch(`projects/${slug}.md`)
  .then(res => {
    if (!res.ok) throw new Error("Article not found");
    return res.text();
  })
  .then(md => {
    document.title = slug.replace(/-/g, " ");
    document.getElementById("content").innerHTML = marked.parse(md);
  })
  .catch(err => {
    document.getElementById("content").innerHTML =
      "<p class='text-red-600'>Article not found.</p>";
    console.error(err);
  });
