import { useState } from "react";
import { getStats, type ShortenResult } from "../api";

interface Props {
  link: ShortenResult;
  fresh: boolean;
}

export function LinkRow({ link, fresh }: Props) {
  const [copied, setCopied] = useState(false);
  const [clicks, setClicks] = useState<number | null>(null);
  const [loadingClicks, setLoadingClicks] = useState(false);

  async function copy() {
    await navigator.clipboard.writeText(link.short_url);
    setCopied(true);
    setTimeout(() => setCopied(false), 1500);
  }

  async function refreshClicks() {
    setLoadingClicks(true);
    try {
      const stats = await getStats(link.code);
      setClicks(stats.clicks);
    } catch {
      setClicks(null);
    } finally {
      setLoadingClicks(false);
    }
  }

  return (
    <div className={`link-row${fresh ? " link-row--fresh" : ""}`}>
      <div className="link-main">
        {/* The short URL is the star: monospace, accent-colored, and the
            primary click target. */}
        <a className="short-url" href={link.short_url} target="_blank" rel="noreferrer">
          {link.short_url.replace(/^https?:\/\//, "")}
        </a>
        <span className="long-url" title={link.long_url}>
          {link.long_url}
        </span>
      </div>

      <div className="link-actions">
        <button className="chip-btn" onClick={refreshClicks} disabled={loadingClicks}>
          {clicks === null ? "clicks?" : `${clicks} clicks`}
        </button>
        <button className="chip-btn chip-btn--primary" onClick={copy}>
          {copied ? "copied ✓" : "copy"}
        </button>
      </div>
    </div>
  );
}
