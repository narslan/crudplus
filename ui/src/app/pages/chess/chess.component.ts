import {
  AfterViewInit,
  ChangeDetectionStrategy,
  Component,
  computed,
  CUSTOM_ELEMENTS_SCHEMA,
  effect,
  signal,
  viewChild,
  ViewChild,
  ViewEncapsulation,
} from '@angular/core';
import { Chess } from 'chess.js';
import { ChessBoardElement} from 'chessboard-element';
//import { ChessBoardElement } from 'chessboard-element';
type orientation = 'white' | 'black';

@Component({
  selector: 'app-chess',
  standalone: true,
  imports: [],
  schemas: [CUSTOM_ELEMENTS_SCHEMA],
  templateUrl: './chess.component.html',
  styleUrl: './chess.component.scss',
  encapsulation: ViewEncapsulation.ShadowDom,
  changeDetection: ChangeDetectionStrategy.OnPush,

})
export class ChessComponent {
  orientation = signal<orientation>('white');
  game = new Chess();
 
  _chessBoard = viewChild.required(ChessBoardElement);

  onDragStart(e: Event) {
    const ce = e as CustomEvent<any>;
    const { piece } = ce.detail;

    if (this.game.isGameOver()) {
      ce.preventDefault();
      return;
    }
    // only pick up pieces for the side to move
    if (
      (this.game.turn() === 'w' && piece.search(/^b/) !== -1) ||
      (this.game.turn() === 'b' && piece.search(/^w/) !== -1)
    ) {
      ce.preventDefault();
      return;
    }
  }

  onSnapEnd(e: Event) {

   computed(() => this._chessBoard().setPosition(this.game.fen()))
  }

  onDrop(ce: Event) {
    const e = ce as CustomEvent<any>;
    const { source, target, setAction } = e.detail;

    try {
      this.game.move({
        from: source,
        to: target,
        promotion: 'q', // NOTE: always promote to a queen
      });
      console.log(this.game.fen());
      
      //  this.ws.next(`${source}${target}`);
      this.updateStatus();
    } catch (error) {
      setAction('snapback');
    }
  }

  updateStatus() {
    let status = '';

    let moveColor = 'White';
    if (this.game.turn() === 'b') {
      moveColor = 'Black';
    }

    if (this.game.isCheckmate()) {
      // checkmate?
      status = `Game over, ${moveColor} is in checkmate.`;
    } else if (this.game.isDraw()) {
      // draw?
      status = 'Game over, drawn position';
    } else {
      // game still on
      status = `${moveColor} to move`;

      // check?
      if (this.game.isCheck()) {
        status += `, ${moveColor} is in check`;
      }
    }

    computed(() => this._chessBoard().setPosition(this.game.fen()))
    //    this._chessBoard.setPosition(this.game.fen());
    //   this._status.innerHTML = status;
    //  this._fen.innerHTML = this.game.fen();
    //this._pgn.innerHTML = this.game.pgn();
  }
}
