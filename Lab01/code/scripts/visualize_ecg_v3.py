import json
import argparse
import matplotlib.pyplot as plt
import pandas as pd


def main():
    parser = argparse.ArgumentParser(
        description="Visualisation ECG + pics (CSV robuste: détecte orientation)"
    )

    parser.add_argument("--csv", required=True, help="Chemin vers le fichier CSV ECG")
    parser.add_argument("--json", required=True, help="Chemin vers le fichier JSON des pics")
    parser.add_argument(
        "--lead",
        type=int,
        default=1,
        help="Index du lead à afficher (0..11). Par défaut: 1 (lead II)"
    )
    parser.add_argument(
        "--samples",
        type=int,
        default=5000,
        help="Nombre d'échantillons à afficher (par défaut: 5000)"
    )

    args = parser.parse_args()

    # Lire le CSV sans hypothèses fragiles
    df = pd.read_csv(args.csv, sep=",", header=None)
    df = df.apply(pd.to_numeric, errors="coerce")
    df = df.dropna(how="all")

    # Si transposé (leads en lignes), on transpose
    # Heuristique: on s'attend à ~12 leads => si nb_lignes petit et nb_colonnes grand -> transpose
    if df.shape[0] <= 20 and df.shape[1] > df.shape[0]:
        df = df.T

    if args.lead < 0 or args.lead >= df.shape[1]:
        raise ValueError(f"Lead index {args.lead} invalide (0..{df.shape[1]-1}), df.shape={df.shape}")

    ecg_signal = df.iloc[:args.samples, args.lead].astype(float).values

    # Charger JSON
    with open(args.json, "r") as f:
        data = json.load(f)

    peaks = data.get("peaks", {})

    def filter_peaks(name):
        return [p for p in peaks.get(name, []) if isinstance(p, int) and 0 <= p < len(ecg_signal)]

    p_peaks = filter_peaks("P")
    q_peaks = filter_peaks("Q")
    r_peaks = filter_peaks("R")
    s_peaks = filter_peaks("S")
    t_peaks = filter_peaks("T")

    # Plot
    plt.figure(figsize=(14, 6))
    plt.plot(ecg_signal, label=f"ECG (lead index {args.lead})", color="blue", linewidth=1)

    if p_peaks:
        plt.scatter(p_peaks, ecg_signal[p_peaks], color="orange", label="P", zorder=3, s=25)
    if q_peaks:
        plt.scatter(q_peaks, ecg_signal[q_peaks], color="purple", label="Q", zorder=3, s=25)
    if r_peaks:
        plt.scatter(r_peaks, ecg_signal[r_peaks], color="red", label="R", zorder=3, s=30)
    if s_peaks:
        plt.scatter(s_peaks, ecg_signal[s_peaks], color="green", label="S", zorder=3, s=25)
    if t_peaks:
        plt.scatter(t_peaks, ecg_signal[t_peaks], color="magenta", label="T", zorder=3, s=25)

    plt.xlabel("Temps (échantillons)")
    plt.ylabel("Amplitude ECG")
    plt.title("ECG avec détection des pics")
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
