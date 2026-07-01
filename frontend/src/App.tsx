import { useState } from "react";
import { ShortenForm } from "./components/ShortenForm";
import { LinkRow } from "./components/LinkRow";
import { shorten, ApiError, type ShortenResult } from "./api";
import "./App.css";

export default function App() {
  const [links, setLinks] = useState<ShortenResult[]>([]);
  const [busy, setBusy] = useState(false);
  const [error, setError] = useState<string | null>(null);

  async function handleShorten(url: string, alias: string) {
    setBusy(true);
    setError(null);
    try {
      const result = await shorten(url, alias || undefined);
      // newest first
      setLinks((prev) => [result, ...prev]);
    } catch (e) {
      // Show the server's message (e.g. "That alias is already taken").
      setError(e instanceof ApiError ? e.message : "Something went wrong");
    } finally {
      setBusy(false);
    }
  }

  return (
    <main className="page">
      <header className="masthead">
        {/* The wordmark leans on the metaphor: a long thing, snipped short. */}
      </header>

      <section className="hero">
        <h1 className="headline">
          Link Shortener <span className="accent"></span>
        </h1>
        <ShortenForm onSubmit={handleShorten} busy={busy} />
        {error && <p className="error" role="alert">{error}</p>}
      </section>

      <section className="results">
        {links.length === 0 ? (
          <p className="empty">Your shortened links will appear here.</p>
        ) : (
          links.map((link, i) => (
            <LinkRow key={link.code} link={link} fresh={i === 0} />
          ))
        )}
      </section>

      <footer className="footer">
        Built with C++ (Drogon), PostgreSQL, React
      </footer>
    </main>
  );
}
