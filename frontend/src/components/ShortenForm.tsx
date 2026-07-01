import { useState, type FormEvent } from "react";

interface Props {
  onSubmit: (url: string, alias: string) => void;
  busy: boolean;
}

export function ShortenForm({ onSubmit, busy }: Props) {
  const [url, setUrl] = useState("");
  const [alias, setAlias] = useState("");
  const [showAlias, setShowAlias] = useState(false);

  function handle(e: FormEvent) {
    e.preventDefault();
    const trimmed = url.trim();
    if (!trimmed || busy) return;
    onSubmit(trimmed, alias.trim());
  }

  return (
    <form className="form" onSubmit={handle}>
      <div className="field">
        <input
          className="url-input"
          type="url"
          inputMode="url"
          placeholder="Paste a long URL…"
          value={url}
          onChange={(e) => setUrl(e.target.value)}
          aria-label="Long URL to shorten"
          autoFocus
        />
        <button className="snip-btn" type="submit" disabled={busy || !url.trim()}>
          {busy ? "Shortening…" : "Shorten"}
        </button>
      </div>

      {showAlias ? (
        <div className="alias-row">
          <span className="alias-prefix">/</span>
          <input
            className="alias-input"
            type="text"
            placeholder="custom-alias (optional)"
            value={alias}
            onChange={(e) => setAlias(e.target.value)}
            aria-label="Custom alias"
          />
        </div>
      ) : (
        <button type="button" className="link-btn" onClick={() => setShowAlias(true)}>
          + add a custom alias
        </button>
      )}
    </form>
  );
}
